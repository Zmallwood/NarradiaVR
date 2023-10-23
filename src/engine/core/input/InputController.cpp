#include "Pch.h"
#include "InputController.h"

namespace nar {
  void InputController::Reset() {
    is_grabbed_ = false;
  }

  bool InputController::IsGrabbedPickResult() {
    auto result = is_grabbed_;
    is_grabbed_ = false;
    return result;
  }
}