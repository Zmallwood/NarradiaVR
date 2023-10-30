/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   struct Player : public Singleton<Player> {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      clock_t ticks_last_movement = 0;
      float movement_speed = 0.1f;
      float movement_step_size = 0.2f;
      float facing_angle_degrees = 0.0f;
      float rotation_speed = 0.5f;
      float rotation_step_size = 5.0f;
      clock_t ticks_last_rotation = 0;
   };
}