/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

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