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

namespace nar {
   /** Registers and provides state for an input controller.
    */
   class HandDevice {
     public:
      bool HasBeenGrabbedPickResult();
      void RegisterBeenGrabbed(bool value);
      void RegisterNotGrabbed();
      void RegisterThumbstickX(float x);
      void RegisterThumbstickY(float x);

      float thumbstick_x() {
         return thumbstick_x_;
      }

      float thumbstick_y() {
         return thumbstick_y_;
      }

     private:
      bool is_grabbed_ = false;
      bool has_been_grabbed_ = false;
      float thumbstick_x_ = 0.0f;
      float thumbstick_y_ = 0.0f;
   };
}