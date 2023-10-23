#include "Pch.h"
#include "SceneManager.h"
#include "xr/program/OpenXrProgram.h"
#include "scenes/intro/scene_IntroScene.h"

namespace nar {
  void SceneManager::InitializeScenes() {
    scenes_.insert({SceneNames::Intro, std::make_shared<IntroScene>()});

    current_scene_ = SceneNames::Intro;
  }

  void SceneManager::UpdateCurrentScene() {
    if (scenes_.count(current_scene_) != 0)
      scenes_.at(current_scene_)->Update();
  }

  void SceneManager::RenderCurrentScene() {
    if (scenes_.count(current_scene_) != 0)
      scenes_.at(current_scene_)->Render();
    OpenXrProgram::Get()->RenderFrame();
  }
}