/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "Pch.h"
#include "module_ThumbstickMovementModule.h"
#include "world/world_Player.h"
#include "engine/core/input/system_ControllerInput.h"

namespace nar {
   void ThumbstickMovementModule::Update() {

      auto delta_t =
          static_cast<float>(clock() - Player::Get()->ticks_last_movement) / CLOCKS_PER_SEC;

      if (delta_t > Player::Get()->movement_speed) {
         auto delta_x = GET(ControllerInput)->left_input_controller()->thumbstick_x();
         auto delta_y = GET(ControllerInput)->left_input_controller()->thumbstick_y();
         auto x_sign = delta_x / std::abs(delta_x);
         auto y_sign = delta_y / std::abs(delta_y);

         if (delta_x != 0 || delta_y != 0) {
            GET(Player)->x += GET(Player)->movement_step_size * x_sign;
            GET(Player)->y += GET(Player)->movement_step_size * y_sign;
            Player::Get()->ticks_last_movement = clock();
         }
      }
   }
}