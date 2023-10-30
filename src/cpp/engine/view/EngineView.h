/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "engine/model/Engine.h"
#include "SceneManagerView.h"

namespace nar {
   class EngineView {
     public:
      EngineView(Engine &engine)
          : engine_(engine),
            scene_manager_view_(engine.scene_manager()) {
      }
      void Render();

     private:
      Engine &engine_;
      SceneManagerView scene_manager_view_;
   };
}