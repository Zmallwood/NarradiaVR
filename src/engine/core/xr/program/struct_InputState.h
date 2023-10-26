#pragma once
#include "const_Side.h"

namespace nar {
   struct InputState {
      XrActionSet action_set = {XR_NULL_HANDLE};
      XrAction grab_action = {XR_NULL_HANDLE};
      XrAction pose_action = {XR_NULL_HANDLE};
      XrAction thumbstick_action_x = {XR_NULL_HANDLE};
      XrAction thumbstick_action_y = {XR_NULL_HANDLE};
      XrAction vibrate_action = {XR_NULL_HANDLE};
      XrAction quit_action = {XR_NULL_HANDLE};
      std::array<XrPath, Side::kCount> hand_subaction_path;
      std::array<XrSpace, Side::kCount> hand_space;
      std::array<float, Side::kCount> hand_scale = {{1.0f, 1.0f}};
      std::array<XrBool32, Side::kCount> hand_active;
   };
}