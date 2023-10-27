/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "Util.h"

namespace nar {
   bool Util::EqualsIgnoreCase(
       const std::string &s1, const std::string &s2, const std::locale &loc) {
      const std::ctype<char> &ctype = std::use_facet<std::ctype<char>>(loc);
      const auto compare_char_lower = [&](char c1, char c2) {
         return ctype.tolower(c1) == ctype.tolower(c2);
      };
      return s1.size() == s2.size() &&
             std::equal(s1.begin(), s1.end(), s2.begin(), compare_char_lower);
   }
}