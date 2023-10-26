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
#include "view/rendering/system_Renderer.h"
#include "model/matter/struct_Cube.h"
#include "controller/system_ControllerInput.h"
#include "model/Engine.h"

namespace nar {
   void MainMenuScene::UpdateDerived() {
      if (ControllerInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         Engine::Get()->scene_manager()->set_new_scene(SceneNames::Main);
      }
   }

   void MainMenuScene::RenderDerived() {
      std::vector<Cube> cubes;
      Renderer::Get()->RenderFrame(cubes);
      Renderer::Get()->RenderFrame(cubes);
   }
}