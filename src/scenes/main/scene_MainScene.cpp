#include "Pch.h"
#include "scene_MainScene.h"
#include "engine/core/xr/program/rendering/Renderer.h"

namespace nar {
  void MainScene::UpdateDerived() {
  }

  void MainScene::RenderDerived() {
    Renderer::Get()->RenderFrame();
  }
}