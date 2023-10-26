#pragma once
#include "matter/struct_Cube.h"

namespace nar {
   class CubeLayer {
     public:
      bool Render(
          XrTime predicted_display_time,
          std::vector<XrCompositionLayerProjectionView> &projection_layer_views,
          std::vector<Cube> cubes_data);

      std::shared_ptr<XrCompositionLayerProjection> layer() {
         return layer_;
      }

     private:
      std::shared_ptr<XrCompositionLayerProjection> layer_ =
          std::make_shared<XrCompositionLayerProjection>();
   };
}