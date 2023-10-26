#pragma once
#include "matter/struct_Cube.h"

namespace nar {
   class Renderer : public Singleton<Renderer> {
     public:
      void RenderFrame(std::vector<Cube> cubes_data);

      XrTime predicted_display_time() {
         return predicted_display_time_;
      }

     private:
      XrTime predicted_display_time_;
   };
}