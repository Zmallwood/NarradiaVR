#pragma once
#include <openxr/openxr_reflection.h>
#include <string>
#include <locale>

namespace nar {
  // Macro to generate stringify functions for OpenXR enumerations based data provided in
  // openxr_reflection.h
  // clang-format off
#define ENUM_CASE_STR(name, val) case name: return #name;
#define MAKE_TO_STRING_FUNC(enumType)                  \
    inline const char* to_string(enumType e) {         \
        switch (e) {                                   \
            XR_LIST_ENUM_##enumType(ENUM_CASE_STR)     \
            default: return "Unknown " #enumType;      \
        }                                              \
    }
  // clang-format on

  MAKE_TO_STRING_FUNC(XrReferenceSpaceType);
  MAKE_TO_STRING_FUNC(XrViewConfigurationType);
  MAKE_TO_STRING_FUNC(XrEnvironmentBlendMode);
  MAKE_TO_STRING_FUNC(XrSessionState);
  MAKE_TO_STRING_FUNC(XrResult);
  MAKE_TO_STRING_FUNC(XrFormFactor);

  bool EqualsIgnoreCase(
      const std::string &s1, const std::string &s2, const std::locale &loc = std::locale());

  std::string Fmt(const char *fmt, ...);
}
#include "Check.h"
