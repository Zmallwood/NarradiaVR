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
#include "scene_MainMenuScene.h"
#include "engine/core/xr/program/rendering/system_Renderer.h"
#include "matter/struct_Cube.h"
#include "engine/core/input/system_ControllerInput.h"
#include "engine/core/system_SceneManager.h"

namespace nar {
   void MainMenuScene::UpdateDerived() {
      if (ControllerInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         SceneManager::Get()->ChangeScene(SceneNames::Main);
      }
   }

   void MainMenuScene::RenderDerived() {
      std::vector<Cube> cubes;
      Renderer::Get()->RenderFrame(cubes);
      Renderer::Get()->RenderFrame(cubes);
   }
}