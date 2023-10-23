#include "Pch.h"
#include "scene_MainMenuScene.h"
#include "engine/core/xr/program/rendering/Renderer.h"
#include "matter/Cube.h"
#include "engine/core/input/ControllerInput.h"
#include "engine/core/SceneManager.h"

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