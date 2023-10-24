#pragma once

namespace nar {
  /**
   * Registers and provides state for an input controller.
   */
  class InputController {
   public:
    bool HasBeenGrabbedPickResult();
    void RegisterBeenGrabbed(bool value);
    void RegisterNotGrabbed();

    bool joystick_moved = false;

   private:
    bool is_grabbed_ = false;
    bool has_been_grabbed_ = false;
  };
}