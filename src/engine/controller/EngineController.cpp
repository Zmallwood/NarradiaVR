/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "Pch.h"
#include "EngineController.h"
#include "engine/model/AndroidVRAppManager.h"
#include "engine/model/OpenXrProgram.h"

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