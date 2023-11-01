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

      private:
        std::string_view type_;
    };
}
