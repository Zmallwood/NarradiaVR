/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "EngineController.h"
#include "engine/model/AndroidVRAppManager.h"
#include "engine/model/OpenXrProgram.h"
#include "engine/model/Engine.h"
#include "SceneManagerController.h"
#include "SystemEventReaderController.h"
#include "SystemEventPollerController.h"
#include "InputActionsController.h"

namespace nar {
   void EngineController::HandleInput() {
      SystemEventReaderController::Get()->ReadSystemEvents();
      SystemEventPollerController::Get()->PollSystemEvents();
      InputActionsController::Get()->PollInputActions();
      SceneManagerController::Get()->HandleInput();
   }

   void EngineController::UpdateGameFlow() {
      if (QuittingGameIfRequested() || ThrottlingGameIfSessionNotRunning())
         Engine::Get()->set_skip_frame(true);
      else
         Engine::Get()->set_skip_frame(false);

      Engine::Get()->set_game_is_running(AndroidVRAppManager::Get()->app()->destroyRequested == 0);

      SceneManagerController::Get()->UpdateGameFlow();
   }

   bool EngineController::QuittingGameIfRequested() {
      if (Engine::Get()->exit_render_loop()) {
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