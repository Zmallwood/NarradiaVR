/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "AppState.h"

namespace nar {
    /**
     * Primarily used to provide access to the android_app object during the games duration.
     */
    class AppManager : public Ston<AppManager> {
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
        AppState appState_;
    };
}
