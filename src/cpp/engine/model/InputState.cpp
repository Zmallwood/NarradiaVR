/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "InputState.h"

namespace nar {
   InputState::~InputState() {
      if (action_set != XR_NULL_HANDLE) {
         for (auto hand : {Side::kLeft, Side::kRight})
            xrDestroySpace(hand_space[hand]);

         xrDestroyActionSet(action_set);
      }
   }
}