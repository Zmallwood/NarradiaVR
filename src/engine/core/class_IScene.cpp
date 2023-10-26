#include "Pch.h"
#include "class_IScene.h"

namespace nar {
   void IScene::Update() {
      UpdateDerived();
   }

   void IScene::Render() {
      RenderDerived();
   }
}