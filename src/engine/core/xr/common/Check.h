#pragma once

#include "Common.h"

#define CHK_STRINGIFY(x) #x
#define TOSTRING(x) CHK_STRINGIFY(x)
#define FILE_AND_LINE __FILE__ ":" TOSTRING(__LINE__)

namespace nar {
  [[noreturn]] inline void Throw(
      std::string failure_message, const char *originator = nullptr,
      const char *source_location = nullptr) {
    if (originator != nullptr)
      failure_message += Fmt("\n    Origin: %s", originator);
    
    if (source_location != nullptr)
      failure_message += Fmt("\n    Source: %s", source_location);

    throw std::logic_error(failure_message);
  }

#define THROW(msg) Throw(msg, nullptr, FILE_AND_LINE);
#define CHECK(exp)                                                                                 \
  {                                                                                                \
    if (!(exp))                                                                          \
      Throw("Check failed", #exp, FILE_AND_LINE);                                                  \
  }
#define CHECK_MSG(exp, msg)                                                                        \
  {                                                                                                \
    if (!(exp))                                                                                \
      Throw(msg, #exp, FILE_AND_LINE);                                                             \
  }

  [[noreturn]] inline void ThrowXrResult(
      XrResult res, const char *originator = nullptr, const char *source_location = nullptr) {
    Throw(Fmt("XrResult failure [%s]", to_string(res)), originator, source_location);
  }

  inline XrResult CheckXrResult(
      XrResult res, const char *originator = nullptr, const char *source_location = nullptr) {
    
    if (XR_FAILED(res))
      ThrowXrResult(res, originator, source_location);

    return res;
  }

#define THROW_XR(xr, cmd) ThrowXrResult(xr, #cmd, FILE_AND_LINE);
#define CHECK_XRCMD(cmd) CheckXrResult(cmd, #cmd, FILE_AND_LINE);
#define CHECK_XRRESULT(res, cmdStr) CheckXrResult(res, cmdStr, FILE_AND_LINE);
}