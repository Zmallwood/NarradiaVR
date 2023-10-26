#pragma once

namespace nar {
   class ThumbstickMovementModule : public Singleton<ThumbstickMovementModule> {
     public:
      void Update();
   };
}