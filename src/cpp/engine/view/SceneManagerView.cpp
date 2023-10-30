/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "SceneManagerView.h"
#include "scenes/intro/view/IntroSceneView.h"
#include "scenes/main_menu/view/MainMenuSceneView.h"
#include "scenes/main/view/MainSceneView.h"
#include "engine/model/SceneManager.h"

namespace nar {
   SceneManagerView::SceneManagerView() {
      scene_views_.insert({SceneNames::Intro, std::make_shared<IntroSceneView>()});
      scene_views_.insert({SceneNames::MainMenu, std::make_shared<MainMenuSceneView>()});
      scene_views_.insert({SceneNames::Main, std::make_shared<MainSceneView>()});
   }

   void SceneManagerView::Render() {

      __android_log_print(
          ANDROID_LOG_INFO, "Narradia", "try render scene, curr_scene: %d",
          SceneManager::Get()->current_scene());
      if (scene_views_.count(SceneManager::Get()->current_scene()) != 0) {
         __android_log_print(ANDROID_LOG_INFO, "Narradia", "do render scene");
         scene_views_.at(SceneManager::Get()->current_scene())->Render();
      }
   }
}