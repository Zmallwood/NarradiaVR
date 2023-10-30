/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "MainMenuSceneView.h"
#include "matter/model/Cube.h"
#include "engine/view/RendererView.h"

namespace nar {
   void MainMenuSceneView::Render() {
      std::vector<Cube> cubes;
      RendererView::Get()->RenderFrame([](XrMatrix4x4f) {});
   }
}