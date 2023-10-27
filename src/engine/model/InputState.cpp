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