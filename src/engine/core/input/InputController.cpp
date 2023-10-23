#include "Pch.h"
#include "InputController.h"

namespace nar {
  bool InputController::HasBeenGrabbedPickResult() {
    auto result = has_been_grabbed_;
    has_been_grabbed_ = false;
    return result;
  }

  void InputController::RegisterBeenGrabbed(bool value) {
    if (!is_grabbed_)
      has_been_grabbed_ = true;
    is_grabbed_ = value;
  }

  void InputController::RegisterNotGrabbed() {
    is_grabbed_ = false;
  }
}