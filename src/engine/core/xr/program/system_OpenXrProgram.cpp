/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_OpenXrProgram.h"
#include "engine/system_OptionsManager.h"
#include "engine/core/xr/options/system_Options.h"
#include "engine/core/xr/graphics/system_GraphicsGL.h"
#include "engine/core/xr/platform/system_AndroidPlatform.h"
#include "engine/core/input/system_ControllerInput.h"
#include "func_GetXrReferenceSpaceCreateInfo.h"
#include "func_GetXrReferenceSpaceCreateInfoTiles.h"
#include "logging/system_ProgramLogger.h"
#include "engine/core/system_GameLoop.h"
#include "input_actions/system_InputActionsHandler.h"
#include <common/xr_linear.h>

namespace nar {
   OpenXrProgram::OpenXrProgram()
       : options_(OptionsManager::Get()->options()),
         platform_plugin_(AndroidPlatform::Get()),
         graphics_plugin_(GraphicsGL::Get()),
         kAcceptableBlendModes{
             XR_ENVIRONMENT_BLEND_MODE_OPAQUE, XR_ENVIRONMENT_BLEND_MODE_ADDITIVE,
             XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND} {
   }

   OpenXrProgram::~OpenXrProgram() {
      if (input_.action_set != XR_NULL_HANDLE) {
         for (auto hand : {Side::kLeft, Side::kRight})
            xrDestroySpace(input_.hand_space[hand]);

         xrDestroyActionSet(input_.action_set);
      }

      for (Swapchain swapchain : swapchains_)
         xrDestroySwapchain(swapchain.handle);

      for (XrSpace visualized_space : visualized_spaces_)
         xrDestroySpace(visualized_space);

      if (app_space_ != XR_NULL_HANDLE)
         xrDestroySpace(app_space_);

      if (session_ != XR_NULL_HANDLE)
         xrDestroySession(session_);

      if (instance_ != XR_NULL_HANDLE)
         xrDestroyInstance(instance_);
   }

   void OpenXrProgram::Init() {
      CreateInstance();
      InitSystem();
      InitDevice();
      InitSession();
      CreateSwapchains();
   }

   void OpenXrProgram::HandleSessionStateChangedEvent(
       const XrEventDataSessionStateChanged &state_changed_event) {
      const XrSessionState old_state = session_state_;
      session_state_ = state_changed_event.state;

      if ((state_changed_event.session != XR_NULL_HANDLE) &&
          (state_changed_event.session != session_)) {
         __android_log_print(
             ANDROID_LOG_ERROR, "Narradia", "XrEventDataSessionStateChanged for unknown session");
         return;
      }

      switch (session_state_) {
      case XR_SESSION_STATE_READY: {
         XrSessionBeginInfo session_begin_info = {XR_TYPE_SESSION_BEGIN_INFO};
         session_begin_info.primaryViewConfigurationType = options_->Parsed.view_config_type;
         xrBeginSession(session_, &session_begin_info);
         session_running_ = true;
         break;
      }
      case XR_SESSION_STATE_STOPPING: {
         session_running_ = false;
         xrEndSession(session_);
         break;
      }
      case XR_SESSION_STATE_EXITING: {
         GameLoop::Get()->set_exit_render_loop(true);
         // Do not attempt to restart because user closed this session.
         GameLoop::Get()->set_request_restart(false);
         break;
      }
      case XR_SESSION_STATE_LOSS_PENDING: {
         GameLoop::Get()->set_exit_render_loop(true);
         // Poll for a new instance.
         GameLoop::Get()->set_request_restart(true);
         break;
      }
      default:
         break;
      }
   }

   void OpenXrProgram::CreateInstanceInternal() {
      // Create union of extensions required by platform and graphics plugins.
      std::vector<const char *> extensions;

      // Transform platform and graphics extension std::strings to C strings.
      const std::vector<std::string> platform_extensions =
          platform_plugin_->GetInstanceExtensions();
      std::transform(
          platform_extensions.begin(), platform_extensions.end(), std::back_inserter(extensions),
          [](const std::string &ext) { return ext.c_str(); });
      const std::vector<std::string> graphics_extensions =
          graphics_plugin_->GetInstanceExtensions();
      std::transform(
          graphics_extensions.begin(), graphics_extensions.end(), std::back_inserter(extensions),
          [](const std::string &ext) { return ext.c_str(); });

      XrInstanceCreateInfo create_info = {XR_TYPE_INSTANCE_CREATE_INFO};
      create_info.next = platform_plugin_->GetInstanceCreateExtension();
      create_info.enabledExtensionCount = (uint32_t)extensions.size();
      create_info.enabledExtensionNames = extensions.data();

      strcpy(create_info.applicationInfo.applicationName, "HelloXR");
      create_info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

      xrCreateInstance(&create_info, &instance_);
   }

   void OpenXrProgram::CreateInstance() {
      ProgramLogger::Get()->LogLayersAndExtensions();
      CreateInstanceInternal();
      ProgramLogger::Get()->LogInstanceInfo();
   }

   XrEnvironmentBlendMode OpenXrProgram::GetPreferredBlendMode() const {
      uint32_t count;
      xrEnumerateEnvironmentBlendModes(
          instance_, system_id_, options_->Parsed.view_config_type, 0, &count, nullptr);

      std::vector<XrEnvironmentBlendMode> blend_modes(count);
      xrEnumerateEnvironmentBlendModes(
          instance_, system_id_, options_->Parsed.view_config_type, count, &count,
          blend_modes.data());

      for (const auto &blend_mode : blend_modes) {
         if (kAcceptableBlendModes.count(blend_mode))
            return blend_mode;
      }

      __android_log_print(
          ANDROID_LOG_ERROR, "Narradia",
          "No acceptable blend mode returned from the xrEnumerateEnvironmentBlendModes.");
   }

   void OpenXrProgram::InitSystem() {
      XrSystemGetInfo system_info = {XR_TYPE_SYSTEM_GET_INFO};
      system_info.formFactor = options_->Parsed.form_factor;
      xrGetSystem(instance_, &system_info, &system_id_);
   }

   void OpenXrProgram::InitDevice() {
      ProgramLogger::Get()->LogViewConfigurations();

      // The graphics API can initialize the graphics device now that the systemId and
      // instance handle are available.
      graphics_plugin_->InitializeDevice(instance_, system_id_);
   }

   void OpenXrProgram::CreateVisualizedSpaces() {
      std::vector<Point> tiles;

      for (auto x = -5; x <= 5; x++) {
         for (auto y = -5; y <= 5; y++)
            tiles.push_back({x, y});
      }

      for (auto tile : tiles) {
         XrReferenceSpaceCreateInfo reference_space_create_info =
             GetXrReferenceSpaceCreateInfoTiles(tile);
         XrSpace space;
         XrResult res = xrCreateReferenceSpace(session_, &reference_space_create_info, &space);

         if (XR_SUCCEEDED(res)) {
            visualized_spaces_.push_back(space);
         }
         else {
            __android_log_print(
                ANDROID_LOG_WARN, "Narradia",
                "Failed to create reference space for a tile with error %d", res);
         }
      }

      // std::string visualized_spaces[] = {"ViewFront",        "Local",      "Stage",
      //                                   "StageLeft",        "StageRight", "StageLeftRotated",
      //                                   "StageRightRotated"};

      // for (const auto &visualized_space : visualized_spaces) {
      //     XrReferenceSpaceCreateInfo reference_space_create_info =
      //         GetXrReferenceSpaceCreateInfo(visualized_space);
      //     XrSpace space;
      //     XrResult res = xrCreateReferenceSpace(session_, &reference_space_create_info,
      //     &space); if (XR_SUCCEEDED(res)) {
      //         visualized_spaces_.push_back(space);
      //     }
      //     else {
      //      __android_log_print(
      //          ANDROID_LOG_WARN, "Narradia",
      //          "Failed to create reference space for a tile with error %d", res);
      //     }
      // }
   }

   void OpenXrProgram::InitSession() {
      {
         XrSessionCreateInfo create_info = {XR_TYPE_SESSION_CREATE_INFO};
         create_info.next = graphics_plugin_->GetGraphicsBinding();
         create_info.systemId = system_id_;
         xrCreateSession(instance_, &create_info, &session_);
      }

      ProgramLogger::Get()->LogReferenceSpaces();
      GET(InputActionsHandler)->InitInputActions();
      CreateVisualizedSpaces();

      {
         XrReferenceSpaceCreateInfo reference_space_create_info =
             GetXrReferenceSpaceCreateInfo(options_->app_space);
         xrCreateReferenceSpace(session_, &reference_space_create_info, &app_space_);
      }
   }

   void OpenXrProgram::CreateSwapchains() {
      // Read graphics properties for preferred swapchain length and logging.
      XrSystemProperties system_properties = {XR_TYPE_SYSTEM_PROPERTIES};
      xrGetSystemProperties(instance_, system_id_, &system_properties);

      // Note: No other view configurations exist at the time this code was written. If
      // this condition is not met, the project will need to be audited to see how support
      // should be added.
      if (options_->Parsed.view_config_type != XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) {
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Unsupported view configuration type.");
         return;
      }

      // Query and cache view configuration views.
      uint32_t view_count;
      xrEnumerateViewConfigurationViews(
          instance_, system_id_, options_->Parsed.view_config_type, 0, &view_count, nullptr);
      config_views_.resize(view_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
      xrEnumerateViewConfigurationViews(
          instance_, system_id_, options_->Parsed.view_config_type, view_count, &view_count,
          config_views_.data());

      // Create and cache view buffer for xrLocateViews later.
      views_.resize(view_count, {XR_TYPE_VIEW});

      // Create the swapchain and get the images.
      if (view_count > 0) {
         // Select a swapchain format.
         uint32_t swapchain_format_count;
         xrEnumerateSwapchainFormats(session_, 0, &swapchain_format_count, nullptr);
         std::vector<int64_t> swapchain_formats(swapchain_format_count);
         xrEnumerateSwapchainFormats(
             session_, (uint32_t)swapchain_formats.size(), &swapchain_format_count,
             swapchain_formats.data());
         color_swapchain_format_ = graphics_plugin_->SelectColorSwapchainFormat(swapchain_formats);

         // Print swapchain formats and the selected one.
         {
            std::string swapchain_formats_string;

            for (int64_t format : swapchain_formats) {
               const bool selected = format == color_swapchain_format_;
               swapchain_formats_string += " ";

               if (selected)
                  swapchain_formats_string += "[";

               swapchain_formats_string += std::to_string(format);

               if (selected)
                  swapchain_formats_string += "]";
            }
         }

         // Create a swapchain for each view.
         for (uint32_t i = 0; i < view_count; i++) {
            const XrViewConfigurationView &vp = config_views_[i];

            // Create the swapchain.
            XrSwapchainCreateInfo swapchain_create_info = {XR_TYPE_SWAPCHAIN_CREATE_INFO};
            swapchain_create_info.arraySize = 1;
            swapchain_create_info.format = color_swapchain_format_;
            swapchain_create_info.width = vp.recommendedImageRectWidth;
            swapchain_create_info.height = vp.recommendedImageRectHeight;
            swapchain_create_info.mipCount = 1;
            swapchain_create_info.faceCount = 1;
            swapchain_create_info.sampleCount =
                graphics_plugin_->GetSupportedSwapchainSampleCount(vp);
            swapchain_create_info.usageFlags =
                XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
            Swapchain swapchain;
            swapchain.width = swapchain_create_info.width;
            swapchain.height = swapchain_create_info.height;
            xrCreateSwapchain(session_, &swapchain_create_info, &swapchain.handle);

            swapchains_.push_back(swapchain);

            uint32_t image_count;
            xrEnumerateSwapchainImages(swapchain.handle, 0, &image_count, nullptr);
            // XXX This should really just return XrSwapchainImageBaseHeader*
            std::vector<XrSwapchainImageBaseHeader *> swapchain_images =
                graphics_plugin_->AllocateSwapchainImageStructs(image_count, swapchain_create_info);
            xrEnumerateSwapchainImages(
                swapchain.handle, image_count, &image_count, swapchain_images[0]);

            swapchain_images_.insert(std::make_pair(swapchain.handle, std::move(swapchain_images)));
         }
      }
   }
}