#include "Pch.h"
#include "MainMenuSceneView.h"
#include "matter/model/Cube.h"
#include "engine.rendering/view/system_Renderer.h"

namespace nar {
   void MainMenuSceneView::Render() {
      std::vector<Cube> cubes;
      Renderer::Get()->RenderFrame(cubes);
   }
}