#pragma once

namespace nar {
    /**
     * Polls input actions for a single hand device (left or right)
     */
    class HandDeviceInputPollController {
      public:
        HandDeviceInputPollController(int hand)
            : hand_(hand) {
        }
        HandDeviceInputPollController() = delete;
        void PollInputActions();

      private:
        int hand_ = -1;
    };
}
