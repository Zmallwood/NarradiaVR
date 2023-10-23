#include "Pch.h"
#include "IScene.h"

namespace nar {
  void IScene::Update() {
    UpdateDerived();
  }

  void IScene::Render() {
    RenderDerived();
  }
}