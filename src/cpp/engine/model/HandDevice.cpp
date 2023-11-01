/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

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
