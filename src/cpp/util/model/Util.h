/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include <locale>
#include <string>

namespace nar {
   class Util {
     public:
      static bool EqualsIgnoreCase(
          const std::string &s1, const std::string &s2, const std::locale &loc = std::locale());
      static std::string_view GetFileExtension(std::string_view abs_file_path);
      static std::string_view GetFileNameNoExt(std::string_view abs_file_path);
   };
}
