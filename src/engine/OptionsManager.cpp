#include "Pch.h"
#include "OptionsManager.h"
#include "core/xr/options/Options.h"

namespace nar {
  bool OptionsManager::Init() {
    options_->SetEnvironmentBlendMode();
    return UpdateOptionsFromSystemProperties();
  }

  bool OptionsManager::UpdateOptionsFromSystemProperties() {
    options_->graphics_plugin = "OpenGLES";

    char value[PROP_VALUE_MAX] = {};
    if (__system_property_get("debug.xr.graphicsPlugin", value) != 0)
      options_->graphics_plugin = value;

    if (__system_property_get("debug.xr.formFactor", value) != 0)
      options_->form_factor = value;

    if (__system_property_get("debug.xr.viewConfiguration", value) != 0)
      options_->view_configuration = value;

    if (__system_property_get("debug.xr.blendMode", value) != 0)
      options_->environment_blend_mode = value;

    try {
      options_->ParseStrings();
    } catch (std::invalid_argument &ia) {
      Log::Write(Log::Level::Error, ia.what());
      return false;
    }
    return true;
  }
}