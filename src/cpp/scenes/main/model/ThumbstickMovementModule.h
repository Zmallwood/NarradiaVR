/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   class ThumbstickMovementModule : public Singleton<ThumbstickMovementModule> {
     public:
      void Update();
   };
}