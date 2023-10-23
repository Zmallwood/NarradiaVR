#pragma once
#include "Level.h"

namespace nar {
  namespace Log {
    void SetLevel(Level min_severity);
    void Write(Level severity, const std::string &msg);
  }
}