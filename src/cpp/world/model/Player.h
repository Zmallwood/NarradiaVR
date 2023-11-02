/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   struct Player : public Singleton<Player> {
      void MoveForward();
      void MoveBackward();
      void MoveRight();
      void MoveLeft();

      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      clock_t ticks_last_movement = 0;
      float movement_speed = 0.05f;
      float movement_step_size = 0.4f;
      float facing_angle_degrees = 0.0f;
      float rotation_speed = 0.5f;
      float rotation_step_size = 5.0f;
      clock_t ticks_last_rotation = 0;

     private:
      void Move(float angle_degrees);
   };
}
