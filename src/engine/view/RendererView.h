/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "matter/model/Cube.h"

namespace nar {
   class RendererView : public Singleton<RendererView> {
     public:
      void RenderFrame(std::vector<Cube> cubes_data);

      XrTime predicted_display_time() {
         return predicted_display_time_;
      }

     private:
      XrTime predicted_display_time_;
   };
}