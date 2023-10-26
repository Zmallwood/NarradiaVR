#include "Pch.h"
#include "SceneManager.h"
#include "model/scenes/intro/scene_IntroScene.h"
#include "model/scenes/main_menu/scene_MainMenuScene.h"
#include "model/scenes/main/scene_MainScene.h"

namespace nar {
   SceneManager::SceneManager() {
      scenes_.insert({SceneNames::Intro, std::make_shared<IntroScene>()});
      scenes_.insert({SceneNames::MainMenu, std::make_shared<MainMenuScene>()});
      scenes_.insert({SceneNames::Main, std::make_shared<MainScene>()});

      new_scene_ = SceneNames::Intro;
   }

   void SceneManager::UpdateGameLogic() {
      if (scenes_.count(current_scene_) != 0)
         scenes_.at(current_scene_)->Update();
   }
}