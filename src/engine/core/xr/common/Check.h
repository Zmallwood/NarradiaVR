#pragma once
#include "CheckMacros.h"

namespace nar {
  [[noreturn]] void
  Throw(std::string failure_message, std::string originator = "", std::string source_location = "");

  [[noreturn]] void
  ThrowXrResult(XrResult res, std::string originator = "", std::string source_location = "");

  XrResult
  CheckXrResult(XrResult res, std::string originator = "", std::string source_location = "");
}