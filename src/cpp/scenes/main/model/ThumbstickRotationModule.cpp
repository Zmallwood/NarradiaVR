/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ThumbstickRotationModule.h"
#include "world/model/Player.h"
#include "engine/model/HandDeviceInput.h"

namespace nar {
   void ThumbstickRotationModule::Update() {
      auto delta_t =
          static_cast<float>(clock() - Player::Get()->ticks_last_rotation) / CLOCKS_PER_SEC;

      if (delta_t > Player::Get()->rotation_speed) {
         auto delta_x = HandDeviceInput::Get()->right_input_device()->thumbstick_x();
         auto x_sign = delta_x / std::abs(delta_x);

         if (std::abs(delta_x) > 0.7f) {
            Player::Get()->facing_angle_degrees -= Player::Get()->rotation_step_size * x_sign;
         }
      }
   }
}
