#pragma once

namespace nar {
    class Mob {
      public:
        Mob(std::string_view type)
            : type_(type) {
        }
        Mob() = delete;

        std::string_view type() {
            return type_;
        }

        clock_t ticks_last_movement = 0;
        float movement_speed = 5.0f;

      private:
        std::string_view type_;
    };
}
