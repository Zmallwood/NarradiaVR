#include "Pch.h"
#include "module_JoystickMovementModule.h"
#include "world/Player.h"
#include "engine/core/input/ControllerInput.h"

namespace nar {
  void JoystickMovementModule::Update() {
    if (GET(ControllerInput)->right_input_controller()->joystick_moved ||
        GET(ControllerInput)->left_input_controller()->joystick_moved) {
      GET(Player)->x += 0.1f;
    }
  }
}