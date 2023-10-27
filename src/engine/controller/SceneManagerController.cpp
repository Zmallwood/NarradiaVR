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
#include "SceneManagerController.h"
#include "scenes/intro/controller/IntroSceneController.h"
#include "scenes/main_menu/controller/MainMenuSceneController.h"
#include "scenes/main/controller/MainSceneController.h"

namespace nar {
   SceneManagerController::SceneManagerController(std::shared_ptr<SceneManager> scene_manager)
       : scene_manager_(scene_manager) {
      scene_controllers_.insert({SceneNames::Intro, std::make_shared<IntroSceneController>()});
      scene_controllers_.insert(
          {SceneNames::MainMenu, std::make_shared<MainMenuSceneController>()});
      scene_controllers_.insert({SceneNames::Main, std::make_shared<MainSceneController>()});
   }

   void SceneManagerController::HandleInput() {
      if (scene_controllers_.count(scene_manager_->current_scene()) != 0)
         scene_controllers_.at(scene_manager_->current_scene())->HandleInput();
   }

   void SceneManagerController::UpdateGameFlow() {
      scene_manager_->set_current_scene(scene_manager_->new_scene());
      if (scene_controllers_.count(scene_manager_->current_scene()) != 0)
         scene_controllers_.at(scene_manager_->current_scene())->UpdateGameFlow();
   }
}