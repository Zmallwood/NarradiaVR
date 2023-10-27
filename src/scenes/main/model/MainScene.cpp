/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "MainScene.h"
#include "module_ThumbstickMovementModule.h"

namespace nar {
   void MainScene::UpdateGameLogic() {
      ThumbstickMovementModule::Get()->Update();
   }
}