/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ThumbstickMovementModule.h"
#include "engine/model/HandDeviceInput.h"
#include "world/model/Player.h"

namespace nar {
    void ThumbstickMovementModule::Update() {
        auto delta_t =
            static_cast<float>(clock() - Player::Get()->ticks_last_movement) / CLOCKS_PER_SEC;

        if (delta_t > 1.0f / Player::Get()->movement_speed) {
            auto delta_x = HandDeviceInput::Get()->left_input_device()->thumbstick_x();
            auto delta_y = HandDeviceInput::Get()->left_input_device()->thumbstick_y();
            auto x_sign = delta_x / std::abs(delta_x);
            auto y_sign = delta_y / std::abs(delta_y);

            if (std::abs(delta_x) > 0.7f || std::abs(delta_y) > 0.7f) {
                Player::Get()->ticks_last_movement = clock();
            }

            if (delta_x > 0.7f) {
                Player::Get()->MoveRight();
            }
            else if (delta_x < -0.7f) {
                Player::Get()->MoveLeft();
            }

            if (delta_y > 0.7f) {
                Player::Get()->MoveForward();
            }
            else if (delta_y < -0.7f) {
                Player::Get()->MoveBackward();
            }
        }
    }
}
