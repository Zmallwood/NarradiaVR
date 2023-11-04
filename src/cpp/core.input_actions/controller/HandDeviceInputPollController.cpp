#include "HandDeviceInputPollController.h"
#include "../../core/model/HandDeviceInput.h"
#include "../../core/model/OpenXrProgram.h"
#include "../model/InputState.h"

namespace nar {
    void HandDeviceInputPollController::PollInputActions() {
        auto session = OpenXrProgram::Get()->session();
        auto input = InputState::Get();

        XrActionStateGetInfo get_info = {XR_TYPE_ACTION_STATE_GET_INFO};
        get_info.action = input->grab_action;
        get_info.subactionPath = input->hand_subaction_path[hand_];

        XrActionStateFloat grab_value = {XR_TYPE_ACTION_STATE_FLOAT};
        xrGetActionStateFloat(session, &get_info, &grab_value);

        if (grab_value.isActive == XR_TRUE) {
            // Scale the rendered hand by 1.0f (open) to 0.5f (fully squeezed).
            input->hand_scale[hand_] = 1.0f - 0.5f * grab_value.currentState;

            if (grab_value.currentState > 0.9f) {
                if (hand_ == kRightHandDevice)
                    HandDeviceInput::Get()->right_input_device()->RegisterBeenGrabbed(true);

                if (hand_ == kLeftHandDevice)
                    HandDeviceInput::Get()->left_input_device()->RegisterBeenGrabbed(true);

                XrHapticVibration vibration = {XR_TYPE_HAPTIC_VIBRATION};
                vibration.amplitude = 0.5;
                vibration.duration = XR_MIN_HAPTIC_DURATION;
                vibration.frequency = XR_FREQUENCY_UNSPECIFIED;

                XrHapticActionInfo haptic_action_info = {XR_TYPE_HAPTIC_ACTION_INFO};
                haptic_action_info.action = input->vibrate_action;
                haptic_action_info.subactionPath = input->hand_subaction_path[hand_];
                xrApplyHapticFeedback(
                    session, &haptic_action_info,
                    reinterpret_cast<XrHapticBaseHeader *>(&vibration));
            }
            else {
                if (hand_ == kRightHandDevice)
                    HandDeviceInput::Get()->right_input_device()->RegisterNotGrabbed();

                if (hand_ == kLeftHandDevice)
                    HandDeviceInput::Get()->left_input_device()->RegisterNotGrabbed();
            }
        }

        get_info.action = input->pose_action;
        XrActionStatePose pose_state = {XR_TYPE_ACTION_STATE_POSE};
        xrGetActionStatePose(session, &get_info, &pose_state);
        input->hand_active[hand_] = pose_state.isActive;

        /////////////// Thumbstick /////////////////////////

        get_info.action = input->thumbstick_action_x;
        get_info.subactionPath = input->hand_subaction_path[hand_];

        XrActionStateFloat thumbstick_x = {XR_TYPE_ACTION_STATE_FLOAT};
        xrGetActionStateFloat(session, &get_info, &thumbstick_x);

        if (thumbstick_x.isActive == XR_TRUE) {
            if (hand_ == kRightHandDevice)
                HandDeviceInput::Get()->right_input_device()->RegisterThumbstickX(
                    thumbstick_x.currentState);

            if (hand_ == kLeftHandDevice)
                HandDeviceInput::Get()->left_input_device()->RegisterThumbstickX(
                    thumbstick_x.currentState);
        }

        get_info.action = input->thumbstick_action_y;
        get_info.subactionPath = input->hand_subaction_path[hand_];

        XrActionStateFloat thumbstick_y = {XR_TYPE_ACTION_STATE_FLOAT};
        xrGetActionStateFloat(session, &get_info, &thumbstick_y);

        if (thumbstick_y.isActive == XR_TRUE) {
            if (hand_ == kRightHandDevice)
                HandDeviceInput::Get()->right_input_device()->RegisterThumbstickY(
                    thumbstick_y.currentState);

            if (hand_ == kLeftHandDevice)
                HandDeviceInput::Get()->left_input_device()->RegisterThumbstickY(
                    thumbstick_y.currentState);
        }

        //////////////////////////////////////////////////
    }
}
