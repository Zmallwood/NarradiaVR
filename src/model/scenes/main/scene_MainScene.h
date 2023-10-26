/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "model/class_IScene.h"

namespace nar {
   class MainScene : public IScene {
     private:
      void UpdateDerived() override;
      void RenderDerived() override;
   };
}