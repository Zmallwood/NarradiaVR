#include "Pch.h"
#include "scene_IntroScene.h"
#include "engine/core/input/ControllerInput.h"
#include "engine/core/SceneManager.h"
#include "engine/core/xr/program/rendering/Renderer.h"

namespace nar {
  void IntroScene::UpdateDerived() {
    if (ControllerInput::Get()->right_input_controller()->IsGrabbedPickResult()) {
      SceneManager::Get()->ChangeScene(SceneNames::MainMenu);
    }
  }

  void IntroScene::RenderDerived() {
    Renderer::Get()->RenderFrame();
  }
}