#pragma once

namespace nar {
  class JoystickMovementModule : public Singleton<JoystickMovementModule> {
   public:
    void Update();
  };
}