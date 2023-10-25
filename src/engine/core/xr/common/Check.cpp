#include "Pch.h"
#include "Check.h"
#include "Common.h"

namespace nar {
  /**
   * Throws an error, used by several macros which provide file name and line number of error.
   */
  [[noreturn]] void
  Throw(std::string failure_message, std::string originator, std::string source_location) {
    if (originator != "")
      failure_message += Fmt("\n    Origin: %s", originator.c_str());

    if (source_location != "")
      failure_message += Fmt("\n    Source: %s", source_location.c_str());

    throw std::logic_error(failure_message);
  }

  /**
   * Specialized version of the Throw-function for XrResult objects specificly.
   */
  [[noreturn]] void
  ThrowXrResult(XrResult res, std::string originator, std::string source_location) {
    Throw(Fmt("XrResult failure [%s]", to_string(res)), originator, source_location);
  }

  /**
   * Throws error if provided XrResult has failed-status.
   */
  XrResult CheckXrResult(XrResult res, std::string originator, std::string source_location) {

    if (XR_FAILED(res))
      ThrowXrResult(res, originator, source_location);

    return res;
  }
}