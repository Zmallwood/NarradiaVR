/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "engine/model/SceneManager.h"
#include "ISceneView.h"
#include "engine/model/SceneNames.h"

namespace nar {
   class SceneManagerView {
     public:
      SceneManagerView(std::shared_ptr<SceneManager> scene_manager);
      void Render();

     private:
      std::shared_ptr<SceneManager> scene_manager_;
      std::map<SceneNames, std::shared_ptr<ISceneView>> scene_views_;
   };
}