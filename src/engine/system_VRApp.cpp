/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_VRApp.h"
#include "core/struct_AndroidAppState.h"

namespace nar {
   /** Process the next main command.
    */
   void VRApp::HandleAppCmd(struct android_app *app, int32_t cmd) {
      AndroidAppState *app_state = (AndroidAppState *)app->userData;

      switch (cmd) {
      /* There is no APP_CMD_CREATE. The ANativeActivity creates the
       * application thread from onCreate(). The application thread
       * then calls android_main().
       */
      case APP_CMD_START: {
         break;
      }
      case APP_CMD_RESUME: {
         app_state->resumed = true;
         break;
      }
      case APP_CMD_PAUSE: {
         app_state->resumed = false;
         break;
      }
      case APP_CMD_STOP: {
         break;
      }
      case APP_CMD_DESTROY: {
         app_state->native_window = NULL;
         break;
      }
      case APP_CMD_INIT_WINDOW: {
         app_state->native_window = app->window;
         break;
      }
      case APP_CMD_TERM_WINDOW: {
         app_state->native_window = NULL;
         break;
      }
      }
   }
}