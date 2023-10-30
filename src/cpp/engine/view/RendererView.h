/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "matter/model/Cube.h"

namespace nar {
   class RendererView : public Singleton<RendererView> {
     public:
      void RenderFrame(std::function<void(XrMatrix4x4f)> gl_render_code);

      XrTime predicted_display_time() {
         return predicted_display_time_;
      }

     private:
      XrTime predicted_display_time_;
   };
}