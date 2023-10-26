#include "Pch.h"
#include "InputActionsController.h"
#include "system/system_OpenXrProgram.h"
#include "system_ControllerInput.h"

#if !defined(XR_USE_PLATFORM_WIN32)
#define strcpy_s(dest, source) strncpy((dest), (source), sizeof(dest))
#endif

namespace nar {
   void InputActionsController::InitInputActions() {
      auto instance = GET(OpenXrProgram)->instance();
      auto input = GET(OpenXrProgram)->input();
      auto session = GET(OpenXrProgram)->session();

      /* Create an action set.
       */
      {
         XrActionSetCreateInfo action_set_info = {XR_TYPE_ACTION_SET_CREATE_INFO};
         strcpy_s(action_set_info.actionSetName, "gameplay");
         strcpy_s(action_set_info.localizedActionSetName, "Gameplay");
         action_set_info.priority = 0;
         xrCreateActionSet(instance, &action_set_info, &input.action_set);
      }

      // Get the XrPath for the left and right hands - we will use them as subaction
      // paths.

      xrStringToPath(instance, "/user/hand/left", &input.hand_subaction_path[Side::kLeft]);

      xrStringToPath(instance, "/user/hand/right", &input.hand_subaction_path[Side::kRight]);

      /* Create actions.
       */
      {
         // Create an input action for grabbing objects with the left and right hands.
         XrActionCreateInfo action_info = {XR_TYPE_ACTION_CREATE_INFO};
         action_info.actionType = XR_ACTION_TYPE_FLOAT_INPUT;
         strcpy_s(action_info.actionName, "grab_object");
         strcpy_s(action_info.localizedActionName, "Grab Object");
         action_info.countSubactionPaths = uint32_t(input.hand_subaction_path.size());
         action_info.subactionPaths = input.hand_subaction_path.data();
         xrCreateAction(input.action_set, &action_info, &input.grab_action);

         // Create an input action getting the left and right hand poses.
         action_info.actionType = XR_ACTION_TYPE_POSE_INPUT;
         strcpy_s(action_info.actionName, "hand_pose");
         strcpy_s(action_info.localizedActionName, "Hand Pose");
         action_info.countSubactionPaths = uint32_t(input.hand_subaction_path.size());
         action_info.subactionPaths = input.hand_subaction_path.data();
         xrCreateAction(input.action_set, &action_info, &input.pose_action);

         // Create an input action getting the left and right joystick orientations.

         action_info = {
             .type = XR_TYPE_ACTION_CREATE_INFO,
             .next = NULL,
             .actionType = XR_ACTION_TYPE_FLOAT_INPUT,
             .countSubactionPaths = Side::kCount,
             .subactionPaths = input.hand_subaction_path.data()};
         strcpy(action_info.actionName, "throttle_x");
         strcpy(action_info.localizedActionName, "Use Throttle left/right");

         xrCreateAction(input.action_set, &action_info, &input.thumbstick_action_x);

         action_info = {
             .type = XR_TYPE_ACTION_CREATE_INFO,
             .next = NULL,
             .actionType = XR_ACTION_TYPE_FLOAT_INPUT,
             .countSubactionPaths = Side::kCount,
             .subactionPaths = input.hand_subaction_path.data()};
         strcpy(action_info.actionName, "throttle_y");
         strcpy(action_info.localizedActionName, "Use Throttle forward/backward");

         xrCreateAction(input.action_set, &action_info, &input.thumbstick_action_y);

         // action_info.actionType = XR_ACTION_TYPE_POSE_INPUT;
         // strcpy_s(action_info.actionName, "primary");
         // strcpy_s(action_info.localizedActionName, "Joystick Orientation");
         // action_info.countSubactionPaths = uint32_t(input.hand_subaction_path.size());
         // action_info.subactionPaths = input.hand_subaction_path.data();
         // xrCreateAction(input.action_set, &action_info, &input.thumbstick_action);

         // Create output actions for vibrating the left and right controller.
         action_info.actionType = XR_ACTION_TYPE_VIBRATION_OUTPUT;
         strcpy_s(action_info.actionName, "vibrate_hand");
         strcpy_s(action_info.localizedActionName, "Vibrate Hand");
         action_info.countSubactionPaths = uint32_t(input.hand_subaction_path.size());
         action_info.subactionPaths = input.hand_subaction_path.data();
         xrCreateAction(input.action_set, &action_info, &input.vibrate_action);

         // Create input actions for quitting the session using the left and right
         // controller. Since it doesn't matter which hand did this, we do not specify
         // subaction paths for it. We will just suggest bindings for both hands, where
         // possible.
         action_info.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
         strcpy_s(action_info.actionName, "quit_session");
         strcpy_s(action_info.localizedActionName, "Quit Session");
         action_info.countSubactionPaths = 0;
         action_info.subactionPaths = nullptr;
         xrCreateAction(input.action_set, &action_info, &input.quit_action);
      }

      std::array<XrPath, Side::kCount> select_path;
      std::array<XrPath, Side::kCount> squeeze_value_path;
      std::array<XrPath, Side::kCount> squeeze_force_path;
      std::array<XrPath, Side::kCount> squeeze_click_path;
      std::array<XrPath, Side::kCount> pose_path;
      std::array<XrPath, Side::kCount> haptic_path;
      std::array<XrPath, Side::kCount> menu_click_path;
      std::array<XrPath, Side::kCount> b_click_path;
      std::array<XrPath, Side::kCount> trigger_value_path;
      std::array<XrPath, Side::kCount> thumbstick_x_path;
      std::array<XrPath, Side::kCount> thumbstick_y_path;

      xrStringToPath(
          instance, "/user/hand/left/input/thumbstick/x", &thumbstick_x_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/thumbstick/x", &thumbstick_x_path[Side::kRight]);

      xrStringToPath(
          instance, "/user/hand/left/input/thumbstick/y", &thumbstick_y_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/thumbstick/y", &thumbstick_y_path[Side::kRight]);

      xrStringToPath(instance, "/user/hand/left/input/select/click", &select_path[Side::kLeft]);
      xrStringToPath(instance, "/user/hand/right/input/select/click", &select_path[Side::kRight]);
      xrStringToPath(
          instance, "/user/hand/left/input/squeeze/value", &squeeze_value_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/squeeze/value", &squeeze_value_path[Side::kRight]);
      xrStringToPath(
          instance, "/user/hand/left/input/squeeze/force", &squeeze_force_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/squeeze/force", &squeeze_force_path[Side::kRight]);
      xrStringToPath(
          instance, "/user/hand/left/input/squeeze/click", &squeeze_click_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/squeeze/click", &squeeze_click_path[Side::kRight]);

      xrStringToPath(instance, "/user/hand/left/input/grip/pose", &pose_path[Side::kLeft]);

      xrStringToPath(instance, "/user/hand/right/input/grip/pose", &pose_path[Side::kRight]);

      xrStringToPath(instance, "/user/hand/left/output/haptic", &haptic_path[Side::kLeft]);

      xrStringToPath(instance, "/user/hand/right/output/haptic", &haptic_path[Side::kRight]);
      xrStringToPath(instance, "/user/hand/left/input/menu/click", &menu_click_path[Side::kLeft]);
      xrStringToPath(instance, "/user/hand/right/input/menu/click", &menu_click_path[Side::kRight]);

      xrStringToPath(instance, "/user/hand/left/input/b/click", &b_click_path[Side::kLeft]);

      xrStringToPath(instance, "/user/hand/right/input/b/click", &b_click_path[Side::kRight]);
      xrStringToPath(
          instance, "/user/hand/left/input/trigger/value", &trigger_value_path[Side::kLeft]);
      xrStringToPath(
          instance, "/user/hand/right/input/trigger/value", &trigger_value_path[Side::kRight]);

      // Suggest bindings for the Oculus Touch.
      {
         XrPath oculus_touch_interaction_profile_path;
         xrStringToPath(
             instance, "/interaction_profiles/oculus/touch_controller",
             &oculus_touch_interaction_profile_path);
         std::vector<XrActionSuggestedBinding> bindings = {
             {{input.grab_action, squeeze_value_path[Side::kLeft]},
              {input.grab_action, squeeze_value_path[Side::kRight]},
              {input.pose_action, pose_path[Side::kLeft]},
              {input.pose_action, pose_path[Side::kRight]},
              {input.thumbstick_action_x, thumbstick_x_path[Side::kLeft]},
              {input.thumbstick_action_x, thumbstick_x_path[Side::kRight]},
              {input.thumbstick_action_y, thumbstick_y_path[Side::kLeft]},
              {input.thumbstick_action_y, thumbstick_y_path[Side::kRight]},
              {input.quit_action, menu_click_path[Side::kLeft]},
              {input.vibrate_action, haptic_path[Side::kLeft]},
              {input.vibrate_action, haptic_path[Side::kRight]}}};
         XrInteractionProfileSuggestedBinding suggested_bindings = {
             XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
         suggested_bindings.interactionProfile = oculus_touch_interaction_profile_path;
         suggested_bindings.suggestedBindings = bindings.data();
         suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
         xrSuggestInteractionProfileBindings(instance, &suggested_bindings);
      }

      XrActionSpaceCreateInfo action_space_info = {XR_TYPE_ACTION_SPACE_CREATE_INFO};
      action_space_info.action = input.pose_action;
      action_space_info.poseInActionSpace.orientation.w = 1.f;
      action_space_info.subactionPath = input.hand_subaction_path[Side::kLeft];
      xrCreateActionSpace(session, &action_space_info, &input.hand_space[Side::kLeft]);
      action_space_info.subactionPath = input.hand_subaction_path[Side::kRight];

      xrCreateActionSpace(session, &action_space_info, &input.hand_space[Side::kRight]);

      XrSessionActionSetsAttachInfo attach_info = {XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
      attach_info.countActionSets = 1;
      attach_info.actionSets = &input.action_set;
      xrAttachSessionActionSets(session, &attach_info);

      GET(OpenXrProgram)->set_input(input);
   }

   void InputActionsController::PollInputActions() {
      auto session = GET(OpenXrProgram)->session();
      auto input = GET(OpenXrProgram)->input();

      input.hand_active = {XR_FALSE, XR_FALSE};

      // Sync actions
      const XrActiveActionSet active_action_set = {input.action_set, XR_NULL_PATH};
      XrActionsSyncInfo sync_info = {XR_TYPE_ACTIONS_SYNC_INFO};
      sync_info.countActiveActionSets = 1;
      sync_info.activeActionSets = &active_action_set;
      xrSyncActions(session, &sync_info);

      // Get pose and grab action state and start haptic vibrate when hand is 90%
      // squeezed.
      for (auto hand : {Side::kLeft, Side::kRight}) {
         XrActionStateGetInfo get_info = {XR_TYPE_ACTION_STATE_GET_INFO};
         get_info.action = input.grab_action;
         get_info.subactionPath = input.hand_subaction_path[hand];

         XrActionStateFloat grab_value = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &grab_value);

         if (grab_value.isActive == XR_TRUE) {
            // Scale the rendered hand by 1.0f (open) to 0.5f (fully squeezed).
            input.hand_scale[hand] = 1.0f - 0.5f * grab_value.currentState;

            if (grab_value.currentState > 0.9f) {

               if (hand == Side::kRight)
                  ControllerInput::Get()->right_input_controller()->RegisterBeenGrabbed(true);

               if (hand == Side::kLeft)
                  ControllerInput::Get()->left_input_controller()->RegisterBeenGrabbed(true);

               XrHapticVibration vibration = {XR_TYPE_HAPTIC_VIBRATION};
               vibration.amplitude = 0.5;
               vibration.duration = XR_MIN_HAPTIC_DURATION;
               vibration.frequency = XR_FREQUENCY_UNSPECIFIED;

               XrHapticActionInfo haptic_action_info = {XR_TYPE_HAPTIC_ACTION_INFO};
               haptic_action_info.action = input.vibrate_action;
               haptic_action_info.subactionPath = input.hand_subaction_path[hand];
               xrApplyHapticFeedback(
                   session, &haptic_action_info, (XrHapticBaseHeader *)&vibration);
            }
            else {
               if (hand == Side::kRight)
                  ControllerInput::Get()->right_input_controller()->RegisterNotGrabbed();

               if (hand == Side::kLeft)
                  ControllerInput::Get()->left_input_controller()->RegisterNotGrabbed();
            }
         }

         get_info.action = input.pose_action;
         XrActionStatePose pose_state = {XR_TYPE_ACTION_STATE_POSE};
         xrGetActionStatePose(session, &get_info, &pose_state);
         input.hand_active[hand] = pose_state.isActive;

         /////////////// JOYSTICK /////////////////////////

         get_info.action = input.thumbstick_action_x;
         get_info.subactionPath = input.hand_subaction_path[hand];

         XrActionStateFloat thumbstick_x = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &thumbstick_x);

         if (thumbstick_x.isActive == XR_TRUE) {
            if (hand == Side::kRight)
               ControllerInput::Get()->right_input_controller()->RegisterThumbstickX(
                   thumbstick_x.currentState);

            if (hand == Side::kLeft)
               ControllerInput::Get()->left_input_controller()->RegisterThumbstickX(
                   thumbstick_x.currentState);
         }

         get_info.action = input.thumbstick_action_y;
         get_info.subactionPath = input.hand_subaction_path[hand];

         XrActionStateFloat thumbstick_y = {XR_TYPE_ACTION_STATE_FLOAT};
         xrGetActionStateFloat(session, &get_info, &thumbstick_y);

         if (thumbstick_y.isActive == XR_TRUE) {
            if (hand == Side::kRight)
               ControllerInput::Get()->right_input_controller()->RegisterThumbstickY(
                   thumbstick_x.currentState);

            if (hand == Side::kLeft)
               ControllerInput::Get()->left_input_controller()->RegisterThumbstickY(
                   thumbstick_x.currentState);
         }

         //////////////////////////////////////////////////
      }

      // There were no subaction paths specified for the quit action, because we don't
      // care which hand did it.
      XrActionStateGetInfo get_info = {
          XR_TYPE_ACTION_STATE_GET_INFO, nullptr, input.quit_action, XR_NULL_PATH};
      XrActionStateBoolean quit_value = {XR_TYPE_ACTION_STATE_BOOLEAN};
      xrGetActionStateBoolean(session, &get_info, &quit_value);

      if ((quit_value.isActive == XR_TRUE) && (quit_value.changedSinceLastSync == XR_TRUE) &&
          (quit_value.currentState == XR_TRUE)) {
         xrRequestExitSession(session);
      }
   }
}