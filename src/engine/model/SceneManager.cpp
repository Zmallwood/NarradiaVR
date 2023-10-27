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
#include "SceneManager.h"
#include "scenes/intro/model/IntroScene.h"
#include "scenes/main_menu/model/MainMenuScene.h"
#include "scenes/main/model/MainScene.h"

namespace nar {
   SceneManager::SceneManager() {
      scenes_.insert({SceneNames::Intro, std::make_shared<IntroScene>()});
      scenes_.insert({SceneNames::MainMenu, std::make_shared<MainMenuScene>()});
      scenes_.insert({SceneNames::Main, std::make_shared<MainScene>()});

      new_scene_ = SceneNames::Intro;
   }

   void SceneManager::UpdateGameLogic() {
      if (scenes_.count(current_scene_) != 0)
         scenes_.at(current_scene_)->UpdateGameLogic();
      // scenes_.at(current_scene_)->Update();
   }
}