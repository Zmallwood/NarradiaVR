#include "Pch.h"
#include "system_ProgramLogger.h"
#include "engine/core/xr/program/system_OpenXrProgram.h"
#include "engine/core/xr/common/func_Common.h"
#include "engine/core/xr/program/func_GetXrVersionString.h"
#include "engine/system_OptionsManager.h"

namespace nar {
   void ProgramLogger::LogInstanceInfo() {
      auto instance = OpenXrProgram::Get()->instance();

      CHECK(instance != XR_NULL_HANDLE);

      XrInstanceProperties instance_properties = {XR_TYPE_INSTANCE_PROPERTIES};
      CHECK_XRCMD(xrGetInstanceProperties(instance, &instance_properties));

      Log::Write(
          Log::Level::Info,
          Fmt("Instance RuntimeName=%s RuntimeVersion=%s", instance_properties.runtimeName,
              GetXrVersionString(instance_properties.runtimeVersion).c_str()));
   }

   void ProgramLogger::LogViewConfigurations() {
      auto instance = OpenXrProgram::Get()->instance();
      auto system_id = OpenXrProgram::Get()->system_id();

      CHECK(instance != XR_NULL_HANDLE);
      CHECK(system_id != XR_NULL_SYSTEM_ID);

      uint32_t view_config_type_count;
      CHECK_XRCMD(
          xrEnumerateViewConfigurations(instance, system_id, 0, &view_config_type_count, nullptr));
      std::vector<XrViewConfigurationType> view_config_types(view_config_type_count);
      CHECK_XRCMD(xrEnumerateViewConfigurations(
          instance, system_id, view_config_type_count, &view_config_type_count,
          view_config_types.data()));
      CHECK((uint32_t)view_config_types.size() == view_config_type_count);

      Log::Write(
          Log::Level::Info,
          Fmt("Available View Configuration Types: (%d)", view_config_type_count));

      for (XrViewConfigurationType view_config_type : view_config_types) {
         Log::Write(
             Log::Level::Verbose,
             Fmt("  View Configuration Type: %s %s", to_string(view_config_type),
                 view_config_type == OptionsManager::Get()->options()->Parsed.view_config_type
                     ? "(Selected)"
                     : ""));

         XrViewConfigurationProperties view_config_properties = {
             XR_TYPE_VIEW_CONFIGURATION_PROPERTIES};
         CHECK_XRCMD(xrGetViewConfigurationProperties(
             instance, system_id, view_config_type, &view_config_properties));

         Log::Write(
             Log::Level::Verbose,
             Fmt("  View configuration FovMutable=%s",
                 view_config_properties.fovMutable == XR_TRUE ? "True" : "False"));

         uint32_t view_count;
         CHECK_XRCMD(xrEnumerateViewConfigurationViews(
             instance, system_id, view_config_type, 0, &view_count, nullptr));

         if (view_count > 0) {
            std::vector<XrViewConfigurationView> views(
                view_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
            CHECK_XRCMD(xrEnumerateViewConfigurationViews(
                instance, system_id, view_config_type, view_count, &view_count, views.data()));

            for (uint32_t i = 0; i < views.size(); i++) {
               const XrViewConfigurationView &view = views[i];

               Log::Write(
                   Log::Level::Verbose,
                   Fmt("    View [%d]: Recommended Width=%d Height=%d SampleCount=%d", i,
                       view.recommendedImageRectWidth, view.recommendedImageRectHeight,
                       view.recommendedSwapchainSampleCount));
               Log::Write(
                   Log::Level::Verbose,
                   Fmt("    View [%d]:     Maximum Width=%d Height=%d SampleCount=%d", i,
                       view.maxImageRectWidth, view.maxImageRectHeight,
                       view.maxSwapchainSampleCount));
            }
         }
         else {
            Log::Write(Log::Level::Error, Fmt("Empty view configuration type"));
         }

         LogEnvironmentBlendMode(view_config_type);
      }
   }

   void ProgramLogger::LogEnvironmentBlendMode(XrViewConfigurationType type) {
      auto instance = OpenXrProgram::Get()->instance();
      auto system_id = OpenXrProgram::Get()->system_id();

      CHECK(instance != XR_NULL_HANDLE);
      CHECK(system_id != 0);

      uint32_t count;
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(instance, system_id, type, 0, &count, nullptr));
      CHECK(count > 0);

      Log::Write(Log::Level::Info, Fmt("Available Environment Blend Mode count : (%d)", count));

      std::vector<XrEnvironmentBlendMode> blend_modes(count);
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(
          instance, system_id, type, count, &count, blend_modes.data()));

      bool blend_mode_found = false;

      for (XrEnvironmentBlendMode mode : blend_modes) {
         const bool blend_mode_match =
             (mode == OptionsManager::Get()->options()->Parsed.environment_blend_mode);
         Log::Write(
             Log::Level::Info, Fmt("Environment Blend Mode (%s) : %s", to_string(mode),
                                   blend_mode_match ? "(Selected)" : ""));
         blend_mode_found |= blend_mode_match;
      }

      CHECK(blend_mode_found);
   }

   void ProgramLogger::LogReferenceSpaces() {
      auto session = OpenXrProgram::Get()->session();

      CHECK(session != XR_NULL_HANDLE);

      uint32_t space_count;
      CHECK_XRCMD(xrEnumerateReferenceSpaces(session, 0, &space_count, nullptr));
      std::vector<XrReferenceSpaceType> spaces(space_count);
      CHECK_XRCMD(xrEnumerateReferenceSpaces(session, space_count, &space_count, spaces.data()));

      Log::Write(Log::Level::Info, Fmt("Available reference spaces: %d", space_count));

      for (XrReferenceSpaceType space : spaces)
         Log::Write(Log::Level::Verbose, Fmt("  Name: %s", to_string(space)));
   }

   void ProgramLogger::LogActionSourceName(XrAction action, const std::string &action_name) const {
      auto session = OpenXrProgram::Get()->session();

      XrBoundSourcesForActionEnumerateInfo get_info = {
          XR_TYPE_BOUND_SOURCES_FOR_ACTION_ENUMERATE_INFO};
      get_info.action = action;
      uint32_t path_count = 0;
      CHECK_XRCMD(xrEnumerateBoundSourcesForAction(session, &get_info, 0, &path_count, nullptr));
      std::vector<XrPath> paths(path_count);
      CHECK_XRCMD(xrEnumerateBoundSourcesForAction(
          session, &get_info, uint32_t(paths.size()), &path_count, paths.data()));

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
         CHECK_XRCMD(xrGetInputSourceLocalizedName(session, &name_info, 0, &size, nullptr));

         if (size < 1)
            continue;

         std::vector<char> grab_source(size);
         CHECK_XRCMD(xrGetInputSourceLocalizedName(
             session, &name_info, uint32_t(grab_source.size()), &size, grab_source.data()));

         if (!source_name.empty())
            source_name += " and ";

         source_name += "'";
         source_name += std::string(grab_source.data(), size - 1);
         source_name += "'";
      }

      Log::Write(
          Log::Level::Info, Fmt("%s action is bound to %s", action_name.c_str(),
                                ((!source_name.empty()) ? source_name.c_str() : "nothing")));
   }

   void ProgramLogger::LogLayersAndExtensions() {
      // Write out extension properties for a given layer.
      const auto log_extensions = [](const char *layerName, int indent = 0) {
         uint32_t instance_extension_count;
         CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(
             layerName, 0, &instance_extension_count, nullptr));
         std::vector<XrExtensionProperties> extensions(
             instance_extension_count, {XR_TYPE_EXTENSION_PROPERTIES});
         CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(
             layerName, (uint32_t)extensions.size(), &instance_extension_count, extensions.data()));

         const std::string indent_str(indent, ' ');
         Log::Write(
             Log::Level::Verbose,
             Fmt("%sAvailable Extensions: (%d)", indent_str.c_str(), instance_extension_count));

         for (const XrExtensionProperties &extension : extensions) {
            Log::Write(
                Log::Level::Verbose, Fmt("%s  Name=%s SpecVersion=%d", indent_str.c_str(),
                                         extension.extensionName, extension.extensionVersion));
         }
      };

      // Log non-layer extensions (layerName==nullptr).
      log_extensions(nullptr);

      // Log layers and any of their extensions.
      {
         uint32_t layer_count;
         CHECK_XRCMD(xrEnumerateApiLayerProperties(0, &layer_count, nullptr));
         std::vector<XrApiLayerProperties> layers(layer_count, {XR_TYPE_API_LAYER_PROPERTIES});
         CHECK_XRCMD(
             xrEnumerateApiLayerProperties((uint32_t)layers.size(), &layer_count, layers.data()));

         Log::Write(Log::Level::Info, Fmt("Available Layers: (%d)", layer_count));

         for (const XrApiLayerProperties &layer : layers) {
            Log::Write(
                Log::Level::Verbose,
                Fmt("  Name=%s SpecVersion=%s LayerVersion=%d Description=%s", layer.layerName,
                    GetXrVersionString(layer.specVersion).c_str(), layer.layerVersion,
                    layer.description));
            log_extensions(layer.layerName, 4);
         }
      }
   }
}