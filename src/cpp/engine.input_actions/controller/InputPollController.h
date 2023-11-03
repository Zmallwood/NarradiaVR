#pragma once
#include "../../engine/model/Constants.h"
#include "HandDeviceInputPollController.h"

namespace nar {
    /**
     * Holds poll controllers for the two hand devices and responsible for polling all input
     * actions.
     */
    class InputPollController : public Singleton<InputPollController> {

      public:
        void PollInputActions();

      private:
        HandDeviceInputPollController left_hand_device_input_poll_controller_{kLeftHandDevice};
        HandDeviceInputPollController right_hand_device_input_poll_controller_{kRightHandDevice};
    };
}
