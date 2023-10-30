/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "ThumbstickMovementModule.h"
#include "world/model/Player.h"
#include "engine/model/HandDeviceInput.h"

namespace nar {
   void ThumbstickMovementModule::Update() {
      auto delta_t =
          static_cast<float>(clock() - Player::Get()->ticks_last_movement) / CLOCKS_PER_SEC;

      if (delta_t > Player::Get()->movement_speed) {
         auto delta_x = HandDeviceInput::Get()->left_input_device()->thumbstick_x();
         auto delta_y = HandDeviceInput::Get()->left_input_device()->thumbstick_y();
         auto x_sign = delta_x / std::abs(delta_x);
         auto y_sign = delta_y / std::abs(delta_y);

         if (std::abs(delta_x) > 0.9f || std::abs(delta_y) > 0.9f) {
            Player::Get()->ticks_last_movement = clock();
         }

         if (std::abs(delta_x) > 0.7f) {
            Player::Get()->x += Player::Get()->movement_step_size * x_sign;
         }

         if (std::abs(delta_y) > 0.7f) {
            Player::Get()->y += Player::Get()->movement_step_size * y_sign;
         }
      }
   }
}