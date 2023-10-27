#include "Pch.h"
#include "SceneManagerView.h"
#include "scenes/intro/view/IntroSceneView.h"
#include "scenes/main_menu/view/MainMenuSceneView.h"
#include "scenes/main/view/MainSceneView.h"

namespace nar {
   SceneManagerView::SceneManagerView(std::shared_ptr<SceneManager> scene_manager)
       : scene_manager_(scene_manager) {
      scene_views_.insert({SceneNames::Intro, std::make_shared<IntroSceneView>()});
      scene_views_.insert({SceneNames::MainMenu, std::make_shared<MainMenuSceneView>()});
      scene_views_.insert({SceneNames::Main, std::make_shared<MainSceneView>()});
   }

   void SceneManagerView::Render() {
      if (scene_manager_->scenes().count(scene_manager_->current_scene()) != 0)
         scene_views_.at(scene_manager_->current_scene())->Render();
   }
}