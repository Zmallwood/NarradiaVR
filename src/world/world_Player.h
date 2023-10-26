#pragma once

namespace nar {
   struct Player : public Singleton<Player> {
      float x = 0.0f;
      float y = 0.0f;
      float z = 0.0f;
      clock_t ticks_last_movement = 0;
      float movement_speed = 0.1f;
      float movement_step_size = 0.2f;
   };
}