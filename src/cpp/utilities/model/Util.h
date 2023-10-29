/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   class Util {
     public:
      static bool EqualsIgnoreCase(
          const std::string &s1, const std::string &s2, const std::locale &loc = std::locale());
      static std::string_view GetFileExtension(std::string_view abs_file_path);
      static std::string_view GetFileNameNoExt(std::string_view abs_file_path);
   };
}