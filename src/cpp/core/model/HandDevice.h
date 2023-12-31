/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
    /**
     * Registers and provides state for an input controller.
     */
    class HandDevice {
      public:
        bool HasBeenGrabbedPickResult();
        void RegisterBeenGrabbed(bool value);
        void RegisterNotGrabbed();
        void RegisterThumbstickX(float x);
        void RegisterThumbstickY(float x);

        float thumbstick_x() {
            return thumbstick_x_;
        }

        float thumbstick_y() {
            return thumbstick_y_;
        }

      private:
        bool is_grabbed_ = false;
        bool has_been_grabbed_ = false;
        float thumbstick_x_ = 0.0f;
        float thumbstick_y_ = 0.0f;
    };
}
