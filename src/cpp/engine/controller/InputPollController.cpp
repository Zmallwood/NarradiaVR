#include "InputPollController.h"
#include "../model/OpenXrProgram.h"

namespace nar {
    /**
     * Responsible for polling all input actions, including hand devices and more.
     */
    void InputPollController::PollInputActions() {

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
        // squeezed for both hand devices.
        left_hand_device_input_poll_controller_.PollInputActions();
        right_hand_device_input_poll_controller_.PollInputActions();

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
