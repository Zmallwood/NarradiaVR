#pragma once

namespace nar {

    class MobGenerator : public Singleton<MobGenerator> {
      public:
        void GenerateMobs();
    };
}
