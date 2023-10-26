#include "Pch.h"
#include "scene_MainMenuScene.h"
#include "engine/core/xr/program/rendering/system_Renderer.h"
#include "matter/struct_Cube.h"
#include "engine/core/input/system_ControllerInput.h"
#include "engine/core/system_SceneManager.h"

namespace nar {
   void MainMenuScene::UpdateDerived() {
      if (ControllerInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         SceneManager::Get()->ChangeScene(SceneNames::Main);
      }
   }

   void MainMenuScene::RenderDerived() {
      std::vector<Cube> cubes;
      Renderer::Get()->RenderFrame(cubes);
      Renderer::Get()->RenderFrame(cubes);
   }
}