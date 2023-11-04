/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "BindingsCreateCtrlr.h"
#include "../../core.input_actions/model/InputState.h"
#include "core/model/OpenXrProgram.h"
#include "../model/Constants.h"

namespace nar {
    /**
     * Create bindings that we want to use.
     */
    void BindingsCreateCtrlr::CreateBindings() {
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
        action_space_info.subactionPath = input->hand_subaction_path[kLeftHandDevice];
        xrCreateActionSpace(session, &action_space_info, &input->hand_space[kLeftHandDevice]);
        action_space_info.subactionPath = input->hand_subaction_path[kRightHandDevice];

        xrCreateActionSpace(session, &action_space_info, &input->hand_space[kRightHandDevice]);

        XrSessionActionSetsAttachInfo attach_info = {XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
        attach_info.countActionSets = 1;
        attach_info.actionSets = &input->action_set;
        xrAttachSessionActionSets(session, &attach_info);
    }

    /**
     * Convert a string to a path object.
     */
    XrPath BindingsCreateCtrlr::CreatePath(std::string path_str) {
        auto instance = OpenXrProgram::Get()->instance();
        XrPath path;
        xrStringToPath(instance, path_str.c_str(), &path);
        return path;
    }
}
