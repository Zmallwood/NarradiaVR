/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
    /**
     * Holds essential data about the android app.
     */
    struct AppState {
        ANativeWindow *native_window = nullptr;
        bool resumed = false;
    };
}
