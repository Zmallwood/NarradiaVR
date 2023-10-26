#pragma once
#include "model/SceneManager.h"

namespace nar {
   class SceneManagerController {
     public:
      SceneManagerController(std::shared_ptr<SceneManager> scene_manager);

      void HandleInput();

      void UpdateGameFlow();

     private:
      std::shared_ptr<SceneManager> scene_manager_;
   };
}