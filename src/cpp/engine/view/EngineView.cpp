/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "EngineView.h"
#include "engine/model/Engine.h"
#include "SceneManagerView.h"

namespace nar {
   void EngineView::Render() {
      __android_log_print(ANDROID_LOG_INFO, "Narradia", "try render engine");
      if (!Engine::Get()->skip_frame()) {
      __android_log_print(ANDROID_LOG_INFO, "Narradia", "do render engine");
         SceneManagerView::Get()->Render();
      }
   }
}