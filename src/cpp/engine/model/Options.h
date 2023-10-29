/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "Pch.h"

namespace nar {
   class Options {
     public:
      std::string graphics_plugin;
      std::string form_factor = "Hmd";
      std::string view_configuration = "Stereo";
      std::string environment_blend_mode = "Opaque";
      std::string app_space = "Local";

      struct {
         XrFormFactor form_factor = {XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY};
         XrViewConfigurationType view_config_type = {XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO};
         XrEnvironmentBlendMode environment_blend_mode = {XR_ENVIRONMENT_BLEND_MODE_OPAQUE};
      } Parsed;

      void ParseStrings();
      std::array<float, 4> GetBackgroundClearColor() const;
      void SetEnvironmentBlendMode();

     private:
      XrFormFactor GetXrFormFactor(const std::string &form_factor_str);
      XrViewConfigurationType GetXrViewConfigurationType(const std::string &view_configuration_str);
      XrEnvironmentBlendMode
      GetXrEnvironmentBlendMode(const std::string &environment_blend_mode_str);
      const char *GetXrEnvironmentBlendModeStr(XrEnvironmentBlendMode environment_blend_mode);
   };
}