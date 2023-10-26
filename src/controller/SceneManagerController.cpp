#include "Pch.h"
#include "SceneManagerController.h"

namespace nar {
   SceneManagerController::SceneManagerController(std::shared_ptr<SceneManager> scene_manager)
       : scene_manager_(scene_manager) {
   }

   void SceneManagerController::HandleInput() {
   }

   void SceneManagerController::UpdateGameFlow() {
      scene_manager_->set_current_scene(scene_manager_->new_scene());
   }
}