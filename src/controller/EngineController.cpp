#include "Pch.h"
#include "EngineController.h"
#include "system/system_AndroidAppManager.h"
#include "system/system_OpenXrProgram.h"

namespace nar {
   void EngineController::HandleInput() {
      system_event_reader_controller_.ReadSystemEvents();
      system_event_poller_controller_.PollSystemEvents();
      input_actions_controller_.PollInputActions();
      scene_manager_controller_.HandleInput();
   }

   void EngineController::UpdateGameFlow() {

      if (QuittingGameIfRequested() || ThrottlingGameIfSessionNotRunning())
         engine_.set_skip_frame(true);
      else
         engine_.set_skip_frame(false);

      engine_.set_game_is_running(GET(AndroidAppManager)->app()->destroyRequested == 0);

      scene_manager_controller_.UpdateGameFlow();
   }

   bool EngineController::QuittingGameIfRequested() {
      if (engine_.exit_render_loop()) {
         ANativeActivity_finish(GET(AndroidAppManager)->app()->activity);
         return true;
      }
      return false;
   }

   bool EngineController::ThrottlingGameIfSessionNotRunning() {
      if (!GET(OpenXrProgram)->IsSessionRunning()) {
         // Throttle loop since xrWaitFrame won't be called.
         std::this_thread::sleep_for(std::chrono::milliseconds(250));
         return true;
      }
      return false;
   }
}