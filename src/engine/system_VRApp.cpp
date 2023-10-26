#include "Pch.h"
#include "system_VRApp.h"
#include "core/struct_AndroidAppState.h"

namespace nar {
   ///
   /// Process the next main command.
   ///
   void VRApp::HandleAppCmd(struct android_app *app, int32_t cmd) {
      AndroidAppState *app_state = (AndroidAppState *)app->userData;

      switch (cmd) {
      // There is no APP_CMD_CREATE. The ANativeActivity creates the
      // application thread from onCreate(). The application thread
      // then calls android_main().
      case APP_CMD_START: {
         Log::Write(Log::Level::Info, "    APP_CMD_START");
         Log::Write(Log::Level::Info, "onStart()");
         break;
      }
      case APP_CMD_RESUME: {
         Log::Write(Log::Level::Info, "onResume()");
         Log::Write(Log::Level::Info, "    APP_CMD_RESUME");
         app_state->resumed = true;
         break;
      }
      case APP_CMD_PAUSE: {
         Log::Write(Log::Level::Info, "onPause()");
         Log::Write(Log::Level::Info, "    APP_CMD_PAUSE");
         app_state->resumed = false;
         break;
      }
      case APP_CMD_STOP: {
         Log::Write(Log::Level::Info, "onStop()");
         Log::Write(Log::Level::Info, "    APP_CMD_STOP");
         break;
      }
      case APP_CMD_DESTROY: {
         Log::Write(Log::Level::Info, "onDestroy()");
         Log::Write(Log::Level::Info, "    APP_CMD_DESTROY");
         app_state->native_window = NULL;
         break;
      }
      case APP_CMD_INIT_WINDOW: {
         Log::Write(Log::Level::Info, "surfaceCreated()");
         Log::Write(Log::Level::Info, "    APP_CMD_INIT_WINDOW");
         app_state->native_window = app->window;
         break;
      }
      case APP_CMD_TERM_WINDOW: {
         Log::Write(Log::Level::Info, "surfaceDestroyed()");
         Log::Write(Log::Level::Info, "    APP_CMD_TERM_WINDOW");
         app_state->native_window = NULL;
         break;
      }
      }
   }
}