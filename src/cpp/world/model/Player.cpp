/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "Player.h"

namespace nar {
   void Player::MoveForward() {
      Move(0);
   }

   void Player::MoveBackward() {
      Move(180.0f);
   }

   void Player::MoveRight() {
      Move(90.0f);
   }

   void Player::MoveLeft() {
      Move(-90.0f);
   }

   void Player::Move(float angle_degrees) {
      auto used_angle = glm::radians(-facing_angle_degrees - 90.0f + angle_degrees);
      auto dx = std::cos(used_angle);
      auto dy = std::sin(used_angle);

      x += dx * movement_step_size;
      y += dy * movement_step_size;
   }
}