#include "Pch.h"
#include "EngineView.h"

namespace nar {
   void EngineView::Render() {
      if (!engine_.skip_frame()) {
         scene_manager_view_.Render();
      }
   }
}