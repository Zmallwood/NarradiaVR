#include "Pch.h"
#include "SceneManagerView.h"

namespace nar {
   void SceneManagerView::Render() {
      if (scene_manager_->scenes().count(scene_manager_->current_scene()) != 0)
         scene_manager_->scenes().at(scene_manager_->current_scene())->Render();
   }
}