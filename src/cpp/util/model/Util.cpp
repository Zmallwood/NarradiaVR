/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Util.h"

namespace nar {
   bool
   Util::EqualsIgnoreCase(const std::string &s1, const std::string &s2, const std::locale &loc) {
      const std::ctype<char> &ctype = std::use_facet<std::ctype<char>>(loc);
      const auto compare_char_lower = [&](char c1, char c2) {
         return ctype.tolower(c1) == ctype.tolower(c2);
      };
      return s1.size() == s2.size() &&
             std::equal(s1.begin(), s1.end(), s2.begin(), compare_char_lower);
   }

   std::string_view Util::GetFileExtension(std::string_view abs_file_path) {
      auto ext_pos = abs_file_path.find_last_of('.') + 1;
      auto file_ext = abs_file_path.substr(ext_pos);
      return file_ext;
   }

   std::string_view Util::GetFileNameNoExt(std::string_view abs_file_path) {
      auto img_name_start = abs_file_path.find_last_of('/') + 1;
      auto image_name_with_ext = abs_file_path.substr(img_name_start);
      auto img_name_ext_pos = image_name_with_ext.find_last_of('.');
      auto file_name_no_ext = image_name_with_ext.substr(0, img_name_ext_pos);
      return file_name_no_ext;
   }
}
