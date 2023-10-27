#include "Pch.h"
#include "MainMenuSceneController.h"
#include "engine/model/HandDeviceInput.h"
#include "engine/model/Engine.h"

namespace nar {
   void MainMenuSceneController::HandleInput() {
   }

   void MainMenuSceneController::UpdateGameFlow() {
      if (HandDeviceInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         Engine::Get()->scene_manager()->set_new_scene(SceneNames::Main);
      }
   }
}