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

#include "Pch.h"
#include "HandDevice.h"

namespace nar {
   /** Tells if the controllers grab-button has been triggered, and resets the state.
    */
   bool HandDevice::HasBeenGrabbedPickResult() {
      auto result = has_been_grabbed_;
      has_been_grabbed_ = false;
      return result;
   }

   /** Called whenever the controllers grab-button is pressed.
    */
   void HandDevice::RegisterBeenGrabbed(bool value) {
      if (!is_grabbed_)
         has_been_grabbed_ = true;
      is_grabbed_ = value;
   }

   /** Called whenever the controllers grab-button is not pressed.
    */
   void HandDevice::RegisterNotGrabbed() {
      is_grabbed_ = false;
   }

   void HandDevice::RegisterThumbstickX(float x) {
      thumbstick_x_ = x;
   }

   void HandDevice::RegisterThumbstickY(float y) {
      thumbstick_y_ = y;
   }
}