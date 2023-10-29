#pragma once

namespace nar {
   class ThumbstickRotationModule : public Singleton<ThumbstickRotationModule> {
     public:
      void Update();
   };
}