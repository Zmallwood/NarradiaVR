/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "engine/model/IScene.h"

namespace nar {
   class MainScene : public IScene {
     private:
      void UpdateGameLogic()  override;
   };
}