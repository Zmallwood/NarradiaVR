#pragma once

namespace nar {
    class CityGenerator : public Singleton<CityGenerator> {
      public:
        void GenerateCity();
    };
}
