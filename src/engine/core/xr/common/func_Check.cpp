#include "Pch.h"
#include "func_Check.h"
#include "func_Common.h"

namespace nar {
   [[noreturn]] void
   Throw(std::string failure_message, std::string originator, std::string source_location) {
      if (originator != "")
         failure_message += Fmt("\n    Origin: %s", originator.c_str());

      if (source_location != "")
         failure_message += Fmt("\n    Source: %s", source_location.c_str());

      throw std::logic_error(failure_message);
   }

   [[noreturn]] void
   ThrowXrResult(XrResult res, std::string originator, std::string source_location) {
      Throw(Fmt("XrResult failure [%s]", to_string(res)), originator, source_location);
   }

   XrResult CheckXrResult(XrResult res, std::string originator, std::string source_location) {

      if (XR_FAILED(res))
         ThrowXrResult(res, originator, source_location);

      return res;
   }
}