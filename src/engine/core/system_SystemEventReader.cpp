/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_SystemEventReader.h"
#include "xr/program/system_OpenXrProgram.h"
#include "engine/system_AndroidAppManager.h"
#include "struct_AndroidAppState.h"

namespace nar {
   void SystemEventReader::ReadSystemEvents() {
      auto app = AndroidAppManager::Get()->app();
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