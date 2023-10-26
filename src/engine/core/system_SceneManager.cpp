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
#include "system_SceneManager.h"
#include "scenes/intro/scene_IntroScene.h"
#include "scenes/main_menu/scene_MainMenuScene.h"
#include "scenes/main/scene_MainScene.h"

namespace nar {
   void SceneManager::InitScenes() {
      scenes_.insert({SceneNames::Intro, std::make_shared<IntroScene>()});
      scenes_.insert({SceneNames::MainMenu, std::make_shared<MainMenuScene>()});
      scenes_.insert({SceneNames::Main, std::make_shared<MainScene>()});

      current_scene_ = SceneNames::Intro;
   }

   void SceneManager::UpdateCurrentScene() {
      if (scenes_.count(current_scene_) != 0)
         scenes_.at(current_scene_)->Update();
   }

   void SceneManager::RenderCurrentScene() {
      if (scenes_.count(current_scene_) != 0)
         scenes_.at(current_scene_)->Render();
   }

   void SceneManager::ChangeScene(SceneNames new_scene) {
      current_scene_ = new_scene;
   }
}