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