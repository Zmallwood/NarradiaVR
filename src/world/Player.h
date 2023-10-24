#pragma once

namespace nar {
  struct Player : public Singleton<Player> {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    int ticks_last_movement = 0;
    int movement_speed = 300;
  };
}