/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "../../core/model/Constants.h"

namespace nar {
    class InputState : public Ston<InputState> {
      public:
        ~InputState();

        XrActionSet action_set = {XR_NULL_HANDLE};
        XrAction grab_action = {XR_NULL_HANDLE};
        XrAction pose_action = {XR_NULL_HANDLE};
        XrAction thumbstick_action_x = {XR_NULL_HANDLE};
        XrAction thumbstick_action_y = {XR_NULL_HANDLE};
        XrAction vibrate_action = {XR_NULL_HANDLE};
        XrAction quit_action = {XR_NULL_HANDLE};
        std::array<XrPath, kNumHandDevices> hand_subaction_path;
        std::array<XrSpace, kNumHandDevices> hand_space;
        std::array<float, kNumHandDevices> hand_scale = {{1.0f, 1.0f}};
        std::array<XrBool32, kNumHandDevices> hand_active;
    };
}
