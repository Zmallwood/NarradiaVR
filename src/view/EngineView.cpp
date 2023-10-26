#include "Pch.h"
#include "EngineView.h"

namespace nar {
   void EngineView::Render() {
      if (!engine_.skip_frame()) {
         // GET(SceneManagerOLD)->RenderCurrentScene();

         scene_manager_view_.Render();
      }
   }
}