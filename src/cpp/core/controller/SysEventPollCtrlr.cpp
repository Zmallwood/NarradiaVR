/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "SysEventPollCtrlr.h"
#include "core/model/Engine.h"
#include "core/model/OpenXrProgram.h"

namespace nar {
    /**
     * Process all pending messages.
     */
    void SysEventPollCtrlr::PollSystemEvents() {
        Engine::Get()->set_exit_render_loop(false);

        while (const XrEventDataBaseHeader *event = TryReadNextEvent()) {
            switch (event->type) {
            case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
                const auto &instance_loss_pending =
                    *reinterpret_cast<const XrEventDataInstanceLossPending *>(event);

                __android_log_print(
                    ANDROID_LOG_WARN, "hello_xr", "XrEventDataInstanceLossPending by %lld",
                    instance_loss_pending.lossTime);

                Engine::Get()->set_exit_render_loop(true);
                return;
            }
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                auto session_state_changed_event =
                    *reinterpret_cast<const XrEventDataSessionStateChanged *>(event);
                OpenXrProgram::Get()->HandleSessionStateChangedEvent(session_state_changed_event);
                break;
            }
            case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED:
                break;
            case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING:
            default: {
                break;
            }
            }
        }
    }

    /**
     * Return event if one is available, otherwise return null.
     */
    const XrEventDataBaseHeader *SysEventPollCtrlr::TryReadNextEvent() {
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
                __android_log_print(
                    ANDROID_LOG_WARN, "Narradia", "%d events lost", eventsLost->lostEventCount);
            }

            return base_header;
        }

        if (xr == XR_EVENT_UNAVAILABLE)
            return nullptr;

        return nullptr;
    }
}
