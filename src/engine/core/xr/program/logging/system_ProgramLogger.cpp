/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_ProgramLogger.h"
#include "engine/core/xr/program/system_OpenXrProgram.h"
#include "engine/system_OptionsManager.h"

namespace nar {
   void ProgramLogger::LogInstanceInfo() {
      auto instance = OpenXrProgram::Get()->instance();

      XrInstanceProperties instance_properties = {XR_TYPE_INSTANCE_PROPERTIES};
      xrGetInstanceProperties(instance, &instance_properties);
   }

   void ProgramLogger::LogViewConfigurations() {
      auto instance = OpenXrProgram::Get()->instance();
      auto system_id = OpenXrProgram::Get()->system_id();

      uint32_t view_config_type_count;

      xrEnumerateViewConfigurations(instance, system_id, 0, &view_config_type_count, nullptr);
      std::vector<XrViewConfigurationType> view_config_types(view_config_type_count);
      xrEnumerateViewConfigurations(
          instance, system_id, view_config_type_count, &view_config_type_count,
          view_config_types.data());

      for (XrViewConfigurationType view_config_type : view_config_types) {
         XrViewConfigurationProperties view_config_properties = {
             XR_TYPE_VIEW_CONFIGURATION_PROPERTIES};
         xrGetViewConfigurationProperties(
             instance, system_id, view_config_type, &view_config_properties);

         uint32_t view_count;
         xrEnumerateViewConfigurationViews(
             instance, system_id, view_config_type, 0, &view_count, nullptr);

         if (view_count > 0) {
            std::vector<XrViewConfigurationView> views(
                view_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
            xrEnumerateViewConfigurationViews(
                instance, system_id, view_config_type, view_count, &view_count, views.data());

            for (uint32_t i = 0; i < views.size(); i++) {
               const XrViewConfigurationView &view = views[i];
            }
         }
         else {
            __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Empty view configuration type");
         }

         LogEnvironmentBlendMode(view_config_type);
      }
   }

   void ProgramLogger::LogEnvironmentBlendMode(XrViewConfigurationType type) {
      auto instance = OpenXrProgram::Get()->instance();
      auto system_id = OpenXrProgram::Get()->system_id();

      uint32_t count;
      xrEnumerateEnvironmentBlendModes(instance, system_id, type, 0, &count, nullptr);

      std::vector<XrEnvironmentBlendMode> blend_modes(count);
      xrEnumerateEnvironmentBlendModes(
          instance, system_id, type, count, &count, blend_modes.data());

      bool blend_mode_found = false;

      for (XrEnvironmentBlendMode mode : blend_modes) {
         const bool blend_mode_match =
             (mode == OptionsManager::Get()->options()->Parsed.environment_blend_mode);
         blend_mode_found |= blend_mode_match;
      }
   }

   void ProgramLogger::LogReferenceSpaces() {
      auto session = OpenXrProgram::Get()->session();

      uint32_t space_count;
      xrEnumerateReferenceSpaces(session, 0, &space_count, nullptr);
      std::vector<XrReferenceSpaceType> spaces(space_count);
      xrEnumerateReferenceSpaces(session, space_count, &space_count, spaces.data());
   }

   void ProgramLogger::LogActionSourceName(XrAction action, const std::string &action_name) const {
      auto session = OpenXrProgram::Get()->session();

      XrBoundSourcesForActionEnumerateInfo get_info = {
          XR_TYPE_BOUND_SOURCES_FOR_ACTION_ENUMERATE_INFO};
      get_info.action = action;
      uint32_t path_count = 0;
      xrEnumerateBoundSourcesForAction(session, &get_info, 0, &path_count, nullptr);
      std::vector<XrPath> paths(path_count);
      xrEnumerateBoundSourcesForAction(
          session, &get_info, uint32_t(paths.size()), &path_count, paths.data());

      std::string source_name;

      for (uint32_t i = 0; i < path_count; ++i) {
         constexpr XrInputSourceLocalizedNameFlags all =
             XR_INPUT_SOURCE_LOCALIZED_NAME_USER_PATH_BIT |
             XR_INPUT_SOURCE_LOCALIZED_NAME_INTERACTION_PROFILE_BIT |
             XR_INPUT_SOURCE_LOCALIZED_NAME_COMPONENT_BIT;

         XrInputSourceLocalizedNameGetInfo name_info = {
             XR_TYPE_INPUT_SOURCE_LOCALIZED_NAME_GET_INFO};
         name_info.sourcePath = paths[i];
         name_info.whichComponents = all;

         uint32_t size = 0;
         xrGetInputSourceLocalizedName(session, &name_info, 0, &size, nullptr);

         if (size < 1)
            continue;

         std::vector<char> grab_source(size);
         xrGetInputSourceLocalizedName(
             session, &name_info, uint32_t(grab_source.size()), &size, grab_source.data());

         if (!source_name.empty())
            source_name += " and ";

         source_name += "'";
         source_name += std::string(grab_source.data(), size - 1);
         source_name += "'";
      }
   }

   void ProgramLogger::LogLayersAndExtensions() {
      // Write out extension properties for a given layer.
      const auto log_extensions = [](const char *layerName, int indent = 0) {
         uint32_t instance_extension_count;
         xrEnumerateInstanceExtensionProperties(layerName, 0, &instance_extension_count, nullptr);
         std::vector<XrExtensionProperties> extensions(
             instance_extension_count, {XR_TYPE_EXTENSION_PROPERTIES});
         xrEnumerateInstanceExtensionProperties(
             layerName, (uint32_t)extensions.size(), &instance_extension_count, extensions.data());

         const std::string indent_str(indent, ' ');
      };

      // Log non-layer extensions (layerName==nullptr).
      log_extensions(nullptr);

      // Log layers and any of their extensions.
      {
         uint32_t layer_count;
         xrEnumerateApiLayerProperties(0, &layer_count, nullptr);
         std::vector<XrApiLayerProperties> layers(layer_count, {XR_TYPE_API_LAYER_PROPERTIES});

         xrEnumerateApiLayerProperties((uint32_t)layers.size(), &layer_count, layers.data());

         for (const XrApiLayerProperties &layer : layers) {
            log_extensions(layer.layerName, 4);
         }
      }
   }
}