/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "SceneManagerController.h"
#include "scenes/intro/controller/IntroSceneController.h"
#include "scenes/main_menu/controller/MainMenuSceneController.h"
#include "scenes/main/controller/MainSceneController.h"

namespace nar {
   SceneManagerController::SceneManagerController() {
      scene_controllers_.insert({SceneNames::Intro, std::make_shared<IntroSceneController>()});
      scene_controllers_.insert(
          {SceneNames::MainMenu, std::make_shared<MainMenuSceneController>()});
      scene_controllers_.insert({SceneNames::Main, std::make_shared<MainSceneController>()});
   }

   void SceneManagerController::HandleInput() {
      if (scene_controllers_.count(SceneManager::Get()->current_scene()) != 0)
         scene_controllers_.at(SceneManager::Get()->current_scene())->HandleInput();
   }

   void SceneManagerController::UpdateGameFlow() {
      SceneManager::Get()->set_current_scene(SceneManager::Get()->new_scene());
      if (scene_controllers_.count(SceneManager::Get()->current_scene()) != 0)
         scene_controllers_.at(SceneManager::Get()->current_scene())->UpdateGameFlow();
   }
}