/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

jhis file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
    /**
     * Creates input actions, primarily to be able to read hand device input:
     */
    class InputActionsCreateController : public Singleton<InputActionsCreateController> {
      public:
        void CreateInputActions();

      private:
        void SetCharArray64(char (&array)[64], std::string text);
        void SetCharArray128(char (&array)[128], std::string text);
    };
}
