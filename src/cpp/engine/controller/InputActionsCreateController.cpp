#include "InputActionsCreateController.h"
#include "../../engine/model/InputState.h"
#include "engine/model/OpenXrProgram.h"

namespace nar {
    /**
     * Create action set and actions that we want to use.
     */
    void InputActionsCreateController::CreateInputActions() {
        auto instance = OpenXrProgram::Get()->instance();
        auto session = OpenXrProgram::Get()->session();
        auto input = InputState::Get();

        // Create an action set.
        XrActionSetCreateInfo action_set_info = {XR_TYPE_ACTION_SET_CREATE_INFO};
        SetCharArray64(action_set_info.actionSetName, "gameplay");
        SetCharArray128(action_set_info.localizedActionSetName, "Gameplay");
        action_set_info.priority = 0;
        xrCreateActionSet(instance, &action_set_info, &input->action_set);

        // Get the XrPath for the left and right hands - we will use them as subaction
        // paths.
        xrStringToPath(instance, "/user/hand/left", &input->hand_subaction_path[Side::kLeft]);
        xrStringToPath(instance, "/user/hand/right", &input->hand_subaction_path[Side::kRight]);

        // Create an input action for grabbing objects with the left and right hands.
        XrActionCreateInfo action_info = {XR_TYPE_ACTION_CREATE_INFO};
        action_info.actionType = XR_ACTION_TYPE_FLOAT_INPUT;
        SetCharArray64(action_info.actionName, "grab_object");
        SetCharArray128(action_info.localizedActionName, "Grab Object");
        action_info.countSubactionPaths = uint32_t(input->hand_subaction_path.size());
        action_info.subactionPaths = input->hand_subaction_path.data();
        xrCreateAction(input->action_set, &action_info, &input->grab_action);

        // Create an input action getting the left and right hand poses.
        action_info.actionType = XR_ACTION_TYPE_POSE_INPUT;
        SetCharArray64(action_info.actionName, "hand_pose");
        SetCharArray128(action_info.localizedActionName, "Hand Pose");
        action_info.countSubactionPaths = uint32_t(input->hand_subaction_path.size());
        action_info.subactionPaths = input->hand_subaction_path.data();
        xrCreateAction(input->action_set, &action_info, &input->pose_action);

        // Create an input action getting the left and right joystick orientations.
        action_info = {
            .type = XR_TYPE_ACTION_CREATE_INFO,
            .next = NULL,
            .actionType = XR_ACTION_TYPE_FLOAT_INPUT,
            .countSubactionPaths = Side::kCount,
            .subactionPaths = input->hand_subaction_path.data()};
        SetCharArray64(action_info.actionName, "throttle_x");
        SetCharArray128(action_info.localizedActionName, "Use Throttle left/right");

        xrCreateAction(input->action_set, &action_info, &input->thumbstick_action_x);

        action_info = {
            .type = XR_TYPE_ACTION_CREATE_INFO,
            .next = NULL,
            .actionType = XR_ACTION_TYPE_FLOAT_INPUT,
            .countSubactionPaths = Side::kCount,
            .subactionPaths = input->hand_subaction_path.data()};
        SetCharArray64(action_info.actionName, "throttle_y");
        SetCharArray128(action_info.localizedActionName, "Use Throttle forward/backward");

        xrCreateAction(input->action_set, &action_info, &input->thumbstick_action_y);

        // Create output actions for vibrating the left and right controller.
        action_info.actionType = XR_ACTION_TYPE_VIBRATION_OUTPUT;
        SetCharArray64(action_info.actionName, "vibrate_hand");
        SetCharArray128(action_info.localizedActionName, "Vibrate Hand");
        action_info.countSubactionPaths = uint32_t(input->hand_subaction_path.size());
        action_info.subactionPaths = input->hand_subaction_path.data();
        xrCreateAction(input->action_set, &action_info, &input->vibrate_action);

        // Create input actions for quitting the session using the left and right
        // controller. Since it doesn't matter which hand did this, we do not specify
        // subaction paths for it. We will just suggest bindings for both hands, where
        // possible.
        action_info.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
        SetCharArray64(action_info.actionName, "quit_session");
        SetCharArray128(action_info.localizedActionName, "Quit Session");
        action_info.countSubactionPaths = 0;
        action_info.subactionPaths = nullptr;
        xrCreateAction(input->action_set, &action_info, &input->quit_action);
    }

    /**
     * Set a char array of size 64 to a text string value.
     */
    void InputActionsCreateController::SetCharArray64(char (&array)[64], std::string text) {
        snprintf(array, sizeof(array), "%s", text.c_str());
    }

    /**
     * Set a char array of size 128 to a text string value.
     */
    void InputActionsCreateController::SetCharArray128(char (&array)[128], std::string text) {
        snprintf(array, sizeof(array), "%s", text.c_str());
    }
}
