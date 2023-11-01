#pragma once

namespace nar {
    class NormalsCalculator : public Singleton<NormalsCalculator> {
      public:
        void CalculateNormals();
    };
}
