/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "EngineView.h"
#include "core/model/Engine.h"
#include "SceneManagerView.h"

namespace nar {
   void EngineView::Render() {
      if (!Engine::Get()->skip_frame()) {
         SceneManagerView::Get()->Render();
      }
   }
}
