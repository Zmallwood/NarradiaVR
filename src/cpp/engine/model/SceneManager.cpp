/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

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
      if (scenes_.count(current_scene_) != 0) {
         scenes_.at(current_scene_)->UpdateGameLogic();
      }
   }
}