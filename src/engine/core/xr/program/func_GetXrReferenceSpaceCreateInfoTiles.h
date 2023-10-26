/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once
#include "func_Math.Pose.h"
#include "world/world_Player.h"

namespace nar {
   inline XrReferenceSpaceCreateInfo GetXrReferenceSpaceCreateInfoTiles(Point coord) {
      XrReferenceSpaceCreateInfo reference_space_create_info = {
          XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
      reference_space_create_info.poseInReferenceSpace = Math::Pose::Translation(
          {static_cast<float>(coord.x) - GET(Player)->x, -2.0f,
           static_cast<float>(coord.y) - GET(Player)->y});
      reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;

      return reference_space_create_info;
   }
}