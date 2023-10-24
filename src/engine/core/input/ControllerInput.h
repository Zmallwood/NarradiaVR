#pragma once
#include "InputController.h"

namespace nar {
  /**
   * Stores input state from the player for the left and right controllers.
   */
  class ControllerInput : public Singleton<ControllerInput> {
   public:
    std::shared_ptr<InputController> right_input_controller() {
      return right_input_controller_;
    }

    std::shared_ptr<InputController> left_input_controller() {
      return left_input_controller_;
    }

   private:
    std::shared_ptr<InputController> right_input_controller_ = std::make_shared<InputController>();
    std::shared_ptr<InputController> left_input_controller_ = std::make_shared<InputController>();
  };
}