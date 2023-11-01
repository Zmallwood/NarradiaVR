/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "InputActionsInitController.h"
#include "../../engine/model/InputState.h"
#include "engine/model/OpenXrProgram.h"

namespace nar {

    /**
     * Initialize input actions and bindings that we want to use.
     */
    auto InputActionsInitController::InitInputActions() -> void {
        CreateActions();
        CreateBindings();
    }

    /**
     * Create action set and actions that we want to use.
     */
    void InputActionsInitController::CreateActions() {
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
     * Create bindings that we want to use.
     */
    void InputActionsInitController::CreateBindings() {
        auto instance = OpenXrProgram::Get()->instance();
        auto session = OpenXrProgram::Get()->session();
        auto input = InputState::Get();

        // Suggest bindings for the Oculus Touch.
        XrPath oculus_touch_interaction_profile_path;
        xrStringToPath(
            instance, "/interaction_profiles/oculus/touch_controller",
            &oculus_touch_interaction_profile_path);
        std::vector<XrActionSuggestedBinding> bindings = {
            {{input->grab_action, CreatePath("/user/hand/left/input/squeeze/value")},
             {input->grab_action, CreatePath("/user/hand/right/input/squeeze/value")},
             {input->pose_action, CreatePath("/user/hand/left/input/grip/pose")},
             {input->pose_action, CreatePath("/user/hand/right/input/grip/pose")},
             {input->thumbstick_action_x, CreatePath("/user/hand/left/input/thumbstick/x")},
             {input->thumbstick_action_x, CreatePath("/user/hand/right/input/thumbstick/x")},
             {input->thumbstick_action_y, CreatePath("/user/hand/left/input/thumbstick/y")},
             {input->thumbstick_action_y, CreatePath("/user/hand/right/input/thumbstick/y")},
             {input->quit_action, CreatePath("/user/hand/left/input/menu/click")},
             {input->vibrate_action, CreatePath("/user/hand/left/output/haptic")},
             {input->vibrate_action, CreatePath("/user/hand/left/output/haptic")}}};
        XrInteractionProfileSuggestedBinding suggested_bindings = {
            XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
        suggested_bindings.interactionProfile = oculus_touch_interaction_profile_path;
        suggested_bindings.suggestedBindings = bindings.data();
        suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
        xrSuggestInteractionProfileBindings(instance, &suggested_bindings);

        XrActionSpaceCreateInfo action_space_info = {XR_TYPE_ACTION_SPACE_CREATE_INFO};
        action_space_info.action = input->pose_action;
        action_space_info.poseInActionSpace.orientation.w = 1.f;
        action_space_info.subactionPath = input->hand_subaction_path[Side::kLeft];
        xrCreateActionSpace(session, &action_space_info, &input->hand_space[Side::kLeft]);
        action_space_info.subactionPath = input->hand_subaction_path[Side::kRight];

        xrCreateActionSpace(session, &action_space_info, &input->hand_space[Side::kRight]);

        XrSessionActionSetsAttachInfo attach_info = {XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
        attach_info.countActionSets = 1;
        attach_info.actionSets = &input->action_set;
        xrAttachSessionActionSets(session, &attach_info);
    }

    /**
     * Convert a string to a path object.
     */
    XrPath InputActionsInitController::CreatePath(std::string path_str) {
        auto instance = OpenXrProgram::Get()->instance();
        XrPath path;
        xrStringToPath(instance, path_str.c_str(), &path);
        return path;
    }
    /**
     * Set a char array of size 64 to a text string value.
     */
    void InputActionsInitController::SetCharArray64(char (&array)[64], std::string text) {
        snprintf(array, sizeof(array), "%s", text.c_str());
    }

    /**
     * Set a char array of size 128 to a text string value.
     */
    void InputActionsInitController::SetCharArray128(char (&array)[128], std::string text) {
        snprintf(array, sizeof(array), "%s", text.c_str());
    }
}
