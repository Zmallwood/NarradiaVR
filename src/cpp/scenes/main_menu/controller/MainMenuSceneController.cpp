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
#include "MainMenuSceneController.h"
#include "engine/model/HandDeviceInput.h"
#include "engine/model/Engine.h"

namespace nar {
   void MainMenuSceneController::HandleInput() {
   }

   void MainMenuSceneController::UpdateGameFlow() {
      if (HandDeviceInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         Engine::Get()->scene_manager()->set_new_scene(SceneNames::Main);
      }
   }
}