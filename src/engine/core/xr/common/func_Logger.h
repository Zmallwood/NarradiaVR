#pragma once
#include "enum_Level.h"

namespace nar {
   namespace Log {
      void Write(Level severity, const std::string &msg);
   }
}