#pragma once

namespace nar {
  class InputController {
   public:
    void Reset();
    bool IsGrabbedPickResult();

    void set_is_grabbed(bool value) {
      is_grabbed_ = value;
    }

   private:
    bool is_grabbed_ = false;
  };
}