#include "Pch.h"
#include "Options.h"
#include "engine/core/xr/common/Common.h"
#include "engine/core/xr/program/OpenXrProgram.h"

namespace nar {
  void Options::ParseStrings() {
    Parsed.form_factor = GetXrFormFactor(form_factor);
    Parsed.view_config_type = GetXrViewConfigurationType(view_configuration);
    Parsed.environment_blend_mode = GetXrEnvironmentBlendMode(environment_blend_mode);
  }

  std::array<float, 4> Options::GetBackgroundClearColor() const {
    static const std::array<float, 4> kMediumBlue{0.0f, 0.5f, 1.0, 1.0f};
    static const std::array<float, 4> kTransparentBlack{0.0f, 0.0f, 0.0f, 0.0f};
    static const std::array<float, 4> kBlack{0.0f, 0.0f, 0.0f, 1.0f};

    switch (Parsed.environment_blend_mode) {
    case XR_ENVIRONMENT_BLEND_MODE_OPAQUE:
      return kMediumBlue;
    case XR_ENVIRONMENT_BLEND_MODE_ADDITIVE:
      return kBlack;
    case XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND:
      return kTransparentBlack;
    default:
      return kMediumBlue;
    }
  }

  void Options::SetEnvironmentBlendMode() {
    environment_blend_mode =
        GetXrEnvironmentBlendModeStr(OpenXrProgram::Get()->GetPreferredBlendMode());
    Parsed.environment_blend_mode = OpenXrProgram::Get()->GetPreferredBlendMode();
  }

  XrFormFactor Options::GetXrFormFactor(const std::string &form_factor_str) {
    if (EqualsIgnoreCase(form_factor_str, "Hmd"))
      return XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

    if (EqualsIgnoreCase(form_factor_str, "Handheld"))
      return XR_FORM_FACTOR_HANDHELD_DISPLAY;

    throw std::invalid_argument(Fmt("Unknown form factor '%s'", form_factor_str.c_str()));
  }

  XrViewConfigurationType
  Options::GetXrViewConfigurationType(const std::string &view_configuration_str) {
    if (EqualsIgnoreCase(view_configuration_str, "Mono"))
      return XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO;

    if (EqualsIgnoreCase(view_configuration_str, "Stereo"))
      return XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    throw std::invalid_argument(
        Fmt("Unknown view configuration '%s'", view_configuration_str.c_str()));
  }

  XrEnvironmentBlendMode
  Options::GetXrEnvironmentBlendMode(const std::string &environment_blend_mode_str) {
    if (EqualsIgnoreCase(environment_blend_mode_str, "Opaque"))
      return XR_ENVIRONMENT_BLEND_MODE_OPAQUE;

    if (EqualsIgnoreCase(environment_blend_mode_str, "Additive"))
      return XR_ENVIRONMENT_BLEND_MODE_ADDITIVE;

    if (EqualsIgnoreCase(environment_blend_mode_str, "AlphaBlend"))
      return XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND;

    throw std::invalid_argument(
        Fmt("Unknown environment blend mode '%s'", environment_blend_mode_str.c_str()));
  }

  const char *Options::GetXrEnvironmentBlendModeStr(XrEnvironmentBlendMode environment_blend_mode) {
    switch (environment_blend_mode) {
    case XR_ENVIRONMENT_BLEND_MODE_OPAQUE:
      return "Opaque";
    case XR_ENVIRONMENT_BLEND_MODE_ADDITIVE:
      return "Additive";
    case XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND:
      return "AlphaBlend";
    default:
      throw std::invalid_argument(
          Fmt("Unknown environment blend mode '%s'", to_string(environment_blend_mode)));
    }
  }
}