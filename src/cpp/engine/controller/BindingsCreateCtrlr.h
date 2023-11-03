/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
    /**
     * Creates bindings primarily to be able to read hand device input.
     */
    class BindingsCreateCtrlr : public Singleton<BindingsCreateCtrlr> {
      public:
        void CreateBindings();

      private:
        XrPath CreatePath(std::string path_str);
    };
}
