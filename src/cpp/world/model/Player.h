/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../core/model/Config.h"

namespace nar {
   struct Player : public Ston<Player> {
      void MoveForward();
      void MoveBackward();
      void MoveRight();
      void MoveLeft();

      float x = 50.0f * Config::Get()->kTileSize;
      float y = 50.0f * Config::Get()->kTileSize;
      float z = 0.0f;
      clock_t ticks_last_movement = 0;
      float movement_speed = 30.0f;
      float movement_step_size = 1.0f;
      float facing_angle_degrees = 0.0f;
      float rotation_speed = 0.5f;
      float rotation_step_size = 5.0f;
      clock_t ticks_last_rotation = 0;

     private:
      void Move(float angle_degrees);
   };
}
