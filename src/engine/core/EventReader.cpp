#include "Pch.h"
#include "EventReader.h"
#include "xr/program/OpenXrProgram.h"
#include "engine/AndroidAppManager.h"
#include "AndroidAppState.h"

namespace nar {
  void EventReader::ReadEvents() {
    auto app = AndroidAppManager::Get()->app();
    auto app_state = static_cast<AndroidAppState *>(app->userData);

    // Read all pending events.
    for (;;) {
      int events;
      struct android_poll_source *source;
      // If the timeout is zero, returns immediately without blocking.
      // If the timeout is negative, waits indefinitely until an event appears.
      const int timout_milliseconds =
          (!app_state->resumed && !OpenXrProgram::Get()->IsSessionRunning() &&
           app->destroyRequested == 0)
              ? -1
              : 0;
      if (ALooper_pollAll(timout_milliseconds, nullptr, &events, (void **)&source) < 0)
        break;

      // Process this event.
      if (source != nullptr)
        source->process(app, source);
    }
  }
}