/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "matter/model/Cube.h"

namespace nar {
   class CubeLayerView {
     public:
      bool Render(
          XrTime predicted_display_time,
          std::vector<XrCompositionLayerProjectionView> &projection_layer_views,
          std::function<void(XrMatrix4x4f)> gl_render_code, Point3F player_translation = {0.0f, 0.0f, 0.0f});

      std::shared_ptr<XrCompositionLayerProjection> layer() {
         return layer_;
      }

     private:
      std::shared_ptr<XrCompositionLayerProjection> layer_ =
          std::make_shared<XrCompositionLayerProjection>();
   };
}
