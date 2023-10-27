/* Copyright 2023 Zmallwood
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#pragma once
#include "HandDevice.h"

namespace nar {
   /** Stores input state from the player for the left and right controllers.
    */
   class HandDeviceInput : public Singleton<HandDeviceInput> {
     public:
      std::shared_ptr<HandDevice> right_input_controller() {
         return right_input_controller_;
      }

      std::shared_ptr<HandDevice> left_input_controller() {
         return left_input_controller_;
      }

     private:
      std::shared_ptr<HandDevice> right_input_controller_ = std::make_shared<HandDevice>();
      std::shared_ptr<HandDevice> left_input_controller_ = std::make_shared<HandDevice>();
   };
}