/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Pch.h"
#include "SystemEventReaderController.h"
#include "engine/model/OpenXrProgram.h"
#include "engine/model/AndroidVRAppManager.h"
#include "engine/model/AndroidAppState.h"

namespace nar {
   void SystemEventReaderController::ReadSystemEvents() {
      auto app = AndroidVRAppManager::Get()->app();
      auto app_state = static_cast<AndroidAppState *>(app->userData);

      /* Read all pending events.
       */
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