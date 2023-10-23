#include "Pch.h"
#include "EventPoller.h"
#include "xr/program/OpenXrProgram.h"
#include "GameLoop.h"
#include "xr/common/Common.h"
#include "xr/program/logging/ProgramLogger.h"
#include "xr/program/OpenXrProgram.h"

namespace nar {
  void EventPoller::PollEvents() {
    auto input = OpenXrProgram::Get()->input();

    GameLoop::Get()->set_exit_render_loop(false);
    GameLoop::Get()->set_request_restart(false);

    // Process all pending messages.
    while (const XrEventDataBaseHeader *event = TryReadNextEvent()) {
      switch (event->type) {
      case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
        const auto &instance_loss_pending =
            *reinterpret_cast<const XrEventDataInstanceLossPending *>(event);
        Log::Write(
            Log::Level::Warning,
            Fmt("XrEventDataInstanceLossPending by %lld", instance_loss_pending.lossTime));

        GameLoop::Get()->set_exit_render_loop(true);
        GameLoop::Get()->set_request_restart(true);
        return;
      }
      case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
        auto session_state_changed_event =
            *reinterpret_cast<const XrEventDataSessionStateChanged *>(event);
        OpenXrProgram::Get()->HandleSessionStateChangedEvent(session_state_changed_event);
        break;
      }
      case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED:
        ProgramLogger::Get()->LogActionSourceName(input.grab_action, "Grab");
        ProgramLogger::Get()->LogActionSourceName(input.quit_action, "Quit");
        ProgramLogger::Get()->LogActionSourceName(input.pose_action, "Pose");
        ProgramLogger::Get()->LogActionSourceName(input.vibrate_action, "Vibrate");
        break;
      case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING:
      default: {
        Log::Write(Log::Level::Verbose, Fmt("Ignoring event type %d", event->type));
        break;
      }
      }
    }
  }

  // Return event if one is available, otherwise return null.
  const XrEventDataBaseHeader *EventPoller::TryReadNextEvent() {
    auto instance = OpenXrProgram::Get()->instance();
    auto event_data_buffer = OpenXrProgram::Get()->event_data_buffer();

    // It is sufficient to clear the just the XrEventDataBuffer header to
    // XR_TYPE_EVENT_DATA_BUFFER
    XrEventDataBaseHeader *base_header =
        reinterpret_cast<XrEventDataBaseHeader *>(&event_data_buffer);
    *base_header = {XR_TYPE_EVENT_DATA_BUFFER};
    const XrResult xr = xrPollEvent(instance, &event_data_buffer);

    if (xr == XR_SUCCESS) {
      if (base_header->type == XR_TYPE_EVENT_DATA_EVENTS_LOST) {
        const XrEventDataEventsLost *const eventsLost =
            reinterpret_cast<const XrEventDataEventsLost *>(base_header);
        Log::Write(Log::Level::Warning, Fmt("%d events lost", eventsLost->lostEventCount));
      }

      return base_header;
    }

    if (xr == XR_EVENT_UNAVAILABLE)
      return nullptr;

    THROW_XR(xr, "xrPollEvent");
  }

}