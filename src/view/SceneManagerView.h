#pragma once
#include "model/SceneManager.h"

namespace nar {
   class SceneManagerView {
     public:
      SceneManagerView(std::shared_ptr<SceneManager> scene_manager)
          : scene_manager_(scene_manager) {
      }

      void Render();

     private:
      std::shared_ptr<SceneManager> scene_manager_;
   };
}