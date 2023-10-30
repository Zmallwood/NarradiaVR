/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "EngineView.h"

namespace nar {
   void EngineView::Render() {
      if (!engine_.skip_frame()) {
         scene_manager_view_.Render();
      }
   }
}