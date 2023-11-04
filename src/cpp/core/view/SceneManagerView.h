/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "ISceneView.h"
#include "core/model/SceneNames.h"

namespace nar {
   class SceneManagerView : public Ston<SceneManagerView> {
     public:
      SceneManagerView();
      void Render();

     private:
      std::map<SceneNames, std::shared_ptr<ISceneView>> scene_views_;
   };
}
