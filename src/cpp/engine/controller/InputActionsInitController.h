/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
    class InputActionsInitController : public Singleton<InputActionsInitController> {
      public:
        auto InitInputActions() -> void;

      private:
        void CreateActions();
        void CreateBindings();
        XrPath CreatePath(std::string path_str);
        void SetCharArray64(char (&array)[64], std::string text);
        void SetCharArray128(char (&array)[128], std::string text);
    };
}
