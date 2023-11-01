/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "InputActionsPollerController.h"
#include "engine/model/OpenXrProgram.h"
#include "engine/model/HandDeviceInput.h"

namespace nar {
   void InputActionsPollerController::PollInputActions() {
      auto session = OpenXrProgram::Get()->session();
      auto input = InputState::Get();

      input->hand_active = {XR_FALSE, XR_FALSE};

      // Sync actions
      const XrActiveActionSet active_action_set = {input->action_set, XR_NULL_PATH};
      XrActionsSyncInfo sync_info = {XR_TYPE_ACTIONS_SYNC_INFO};
      sync_info.countActiveActionSets = 1;
      sync_info.activeActionSets = &active_action_set;
      xrSyncActions(session, &sync_info);

      // Get pose and grab action state and start haptic vibrate when hand is 90%
      // squeezed.
      for (auto hand : {Side::kLeft, Side::kRight}) {
         XrActionStateGetInfo get_info = {XR_TYPE_ACTION_STATE_GET_INFO};
         get_info.action = input->grab_action;
         get_info.subactionPath = input->hand_subaction_path[hand];

         XrActionStateFloat grab_value = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &grab_value);

         if (grab_value.isActive == XR_TRUE) {
            // Scale the rendered hand by 1.0f (open) to 0.5f (fully squeezed).
            input->hand_scale[hand] = 1.0f - 0.5f * grab_value.currentState;

            if (grab_value.currentState > 0.9f) {
               if (hand == Side::kRight)
                  HandDeviceInput::Get()->right_input_device()->RegisterBeenGrabbed(true);

               if (hand == Side::kLeft)
                  HandDeviceInput::Get()->left_input_device()->RegisterBeenGrabbed(true);

               XrHapticVibration vibration = {XR_TYPE_HAPTIC_VIBRATION};
               vibration.amplitude = 0.5;
               vibration.duration = XR_MIN_HAPTIC_DURATION;
               vibration.frequency = XR_FREQUENCY_UNSPECIFIED;

               XrHapticActionInfo haptic_action_info = {XR_TYPE_HAPTIC_ACTION_INFO};
               haptic_action_info.action = input->vibrate_action;
               haptic_action_info.subactionPath = input->hand_subaction_path[hand];
               xrApplyHapticFeedback(
                   session, &haptic_action_info, (XrHapticBaseHeader *)&vibration);
            }
            else {
               if (hand == Side::kRight)
                  HandDeviceInput::Get()->right_input_device()->RegisterNotGrabbed();

               if (hand == Side::kLeft)
                  HandDeviceInput::Get()->left_input_device()->RegisterNotGrabbed();
            }
         }

         get_info.action = input->pose_action;
         XrActionStatePose pose_state = {XR_TYPE_ACTION_STATE_POSE};
         xrGetActionStatePose(session, &get_info, &pose_state);
         input->hand_active[hand] = pose_state.isActive;

         /////////////// JOYSTICK /////////////////////////

         get_info.action = input->thumbstick_action_x;
         get_info.subactionPath = input->hand_subaction_path[hand];

         XrActionStateFloat thumbstick_x = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &thumbstick_x);

         if (thumbstick_x.isActive == XR_TRUE) {
            if (hand == Side::kRight)
               HandDeviceInput::Get()->right_input_device()->RegisterThumbstickX(
                   thumbstick_x.currentState);

            if (hand == Side::kLeft)
               HandDeviceInput::Get()->left_input_device()->RegisterThumbstickX(
                   thumbstick_x.currentState);
         }

         get_info.action = input->thumbstick_action_y;
         get_info.subactionPath = input->hand_subaction_path[hand];

         XrActionStateFloat thumbstick_y = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &thumbstick_y);

         if (thumbstick_y.isActive == XR_TRUE) {
            if (hand == Side::kRight)
               HandDeviceInput::Get()->right_input_device()->RegisterThumbstickY(
                   thumbstick_y.currentState);

            if (hand == Side::kLeft)
               HandDeviceInput::Get()->left_input_device()->RegisterThumbstickY(
                   thumbstick_y.currentState);
         }

         //////////////////////////////////////////////////
      }

      // There were no subaction paths specified for the quit action, because we don't
      // care which hand did it.
      XrActionStateGetInfo get_info = {
          XR_TYPE_ACTION_STATE_GET_INFO, nullptr, input->quit_action, XR_NULL_PATH};
      XrActionStateBoolean quit_value = {XR_TYPE_ACTION_STATE_BOOLEAN};
      xrGetActionStateBoolean(session, &get_info, &quit_value);

      if ((quit_value.isActive == XR_TRUE) && (quit_value.changedSinceLastSync == XR_TRUE) &&
          (quit_value.currentState == XR_TRUE)) {
         xrRequestExitSession(session);
      }
   }
}
