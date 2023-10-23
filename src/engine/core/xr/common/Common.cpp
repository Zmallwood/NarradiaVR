#include "Pch.h"
#include "Common.h"
#include <algorithm>
#include <memory>
#include <stdarg.h>
#include <stddef.h>

namespace nar {
  bool EqualsIgnoreCase(const std::string &s1, const std::string &s2, const std::locale &loc) {
    const std::ctype<char> &ctype = std::use_facet<std::ctype<char>>(loc);
    const auto compare_char_lower = [&](char c1, char c2) {
      return ctype.tolower(c1) == ctype.tolower(c2);
    };
    return s1.size() == s2.size() &&
           std::equal(s1.begin(), s1.end(), s2.begin(), compare_char_lower);
  }

  std::string Fmt(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    int size = std::vsnprintf(nullptr, 0, fmt, vl);
    va_end(vl);

    if (size != -1) {
      std::unique_ptr<char[]> buffer(new char[size + 1]);

      va_start(vl, fmt);
      size = std::vsnprintf(buffer.get(), size + 1, fmt, vl);
      va_end(vl);

      if (size != -1)
        return std::string(buffer.get(), size);
    }

    throw std::runtime_error("Unexpected vsnprintf failure");
  }
}