/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "AndroidAppState.h"

namespace nar {
   class AndroidVRAppManager : public Singleton<AndroidVRAppManager> {
     public:
      void Init();
      void Cleanup();

      android_app *app() {
         return app_;
      }

      void set_app(android_app *value) {
         app_ = value;
      }

     private:
      static void HandleAppCmd(android_app *app, int32_t cmd);

      android_app *app_;
      AndroidAppState appState_;
   };
}