/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "engine/model/SceneManager.h"
#include "ISceneController.h"
#include "engine/model/SceneNames.h"

namespace nar {
   class SceneManagerController {
     public:
      SceneManagerController(std::shared_ptr<SceneManager> scene_manager);
      void HandleInput();
      void UpdateGameFlow();

     private:
      std::shared_ptr<SceneManager> scene_manager_;
      std::map<SceneNames, std::shared_ptr<ISceneController>> scene_controllers_;
   };
}