/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "EngineController.h"
#include "engine/model/AndroidVRAppManager.h"
#include "engine/model/OpenXrProgram.h"

namespace nar {
   EngineController::EngineController(Engine &engine)
       : engine_(engine),
         scene_manager_controller_(engine.scene_manager()) {
   }

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

      engine_.set_game_is_running(AndroidVRAppManager::Get()->app()->destroyRequested == 0);

      scene_manager_controller_.UpdateGameFlow();
   }

   bool EngineController::QuittingGameIfRequested() {
      if (engine_.exit_render_loop()) {
         ANativeActivity_finish(AndroidVRAppManager::Get()->app()->activity);
         return true;
      }
      return false;
   }

   bool EngineController::ThrottlingGameIfSessionNotRunning() {
      if (!OpenXrProgram::Get()->IsSessionRunning()) {
         // Throttle loop since xrWaitFrame won't be called.
         std::this_thread::sleep_for(std::chrono::milliseconds(250));
         return true;
      }
      return false;
   }
}