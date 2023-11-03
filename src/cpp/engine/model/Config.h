#pragma once

namespace nar {
    /**
     * Configuration constants for mixed purposes.
     */
    class Config : public Singleton<Config> {
      public:
        constexpr static float kTileSize = 3.0f;
        constexpr static float kElevAmount = 1.5f;
    };
}
