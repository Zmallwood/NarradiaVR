#pragma once

namespace nar {
   class Util {
     public:
      static bool EqualsIgnoreCase(
          const std::string &s1, const std::string &s2, const std::locale &loc = std::locale());
   };
}