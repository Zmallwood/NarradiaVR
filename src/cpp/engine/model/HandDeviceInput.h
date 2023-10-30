/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "HandDevice.h"

namespace nar {
   /** Stores input state from the player for the left and right controllers.
    */
   class HandDeviceInput : public Singleton<HandDeviceInput> {
     public:
      std::shared_ptr<HandDevice> right_input_device() {
         return right_input_device_;
      }

      std::shared_ptr<HandDevice> left_input_device() {
         return left_input_device_;
      }

     private:
      std::shared_ptr<HandDevice> right_input_device_ = std::make_shared<HandDevice>();
      std::shared_ptr<HandDevice> left_input_device_ = std::make_shared<HandDevice>();
   };
}