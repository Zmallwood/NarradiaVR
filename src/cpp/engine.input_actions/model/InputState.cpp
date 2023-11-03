/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "InputState.h"

namespace nar {
   InputState::~InputState() {
      if (action_set != XR_NULL_HANDLE) {
         for (auto hand : {kLeftHandDevice, kRightHandDevice})
            xrDestroySpace(hand_space[hand]);

         xrDestroyActionSet(action_set);
      }
   }
}
