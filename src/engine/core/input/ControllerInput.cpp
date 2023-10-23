#include "Pch.h"
#include "ControllerInput.h"

namespace nar {
  void ControllerInput::Reset() {
    right_input_controller_->Reset();
    left_input_controller_->Reset();
  }
}