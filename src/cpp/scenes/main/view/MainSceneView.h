/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "engine/view/ISceneView.h"

namespace nar {
   class MainSceneView : public ISceneView {
     public:
      MainSceneView();
      void Render() override;

     private:
      std::map<int, std::map<int, RenderId>> rendids_tiles;
   };
}