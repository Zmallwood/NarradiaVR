#pragma once

namespace nar {
    class MobMovementModule : public Singleton<MobMovementModule> {
      public:
        void Update();
    };
}
