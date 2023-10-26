#pragma once
#include "macros_CheckMacros.h"

namespace nar {
   /**
    * Throws an error, used by several macros which provide file name and line number of error.
    */
   [[noreturn]] void Throw(
       std::string failure_message, std::string originator = "", std::string source_location = "");

   /**
    * Specialized version of the Throw-function for XrResult objects specificly.
    */
   [[noreturn]] void
   ThrowXrResult(XrResult res, std::string originator = "", std::string source_location = "");

   /**
    * Throws error if provided XrResult has failed-status.
    */
   XrResult
   CheckXrResult(XrResult res, std::string originator = "", std::string source_location = "");
}