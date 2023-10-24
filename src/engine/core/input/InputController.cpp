#include "Pch.h"
#include "InputController.h"

namespace nar {
  /**
   * Tells if the controllers grab-button has been triggered, and resets the state.
   **/
  bool InputController::HasBeenGrabbedPickResult() {
    auto result = has_been_grabbed_;
    has_been_grabbed_ = false;
    return result;
  }

  /**
   * Called whenever the controllers grab-button is pressed.
   */
  void InputController::RegisterBeenGrabbed(bool value) {
    if (!is_grabbed_)
      has_been_grabbed_ = true;
    is_grabbed_ = value;
  }

  /**
   * Called whenever the controllers grab-button is not pressed.
   */
  void InputController::RegisterNotGrabbed() {
    is_grabbed_ = false;
  }
}