/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "../../../engine/view/ISceneView.h"

namespace nar {
   class IntroSceneView : public ISceneView {
     public:
      void Render() override;
   };
}
