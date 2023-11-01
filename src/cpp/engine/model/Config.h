#pragma once

namespace nar {
    class Config : public Singleton<Config> {
      public:
        constexpr static float kTileSize = 2.0f;
        constexpr static float kElevAmount = 0.4f;
    };
}
