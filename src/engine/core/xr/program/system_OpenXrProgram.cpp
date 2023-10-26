#include "Pch.h"
#include "system_OpenXrProgram.h"
#include "engine/system_OptionsManager.h"
#include "engine/core/xr/common/func_Common.h"
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
#include <array>
#include <cmath>
#include <set>

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

      Log::Write(
          Log::Level::Info,
          Fmt("XrEventDataSessionStateChanged: state %s->%s session=%lld time=%lld",
              to_string(old_state), to_string(session_state_), state_changed_event.session,
              state_changed_event.time));

      if ((state_changed_event.session != XR_NULL_HANDLE) &&
          (state_changed_event.session != session_)) {
         Log::Write(Log::Level::Error, "XrEventDataSessionStateChanged for unknown session");
         return;
      }

      switch (session_state_) {
      case XR_SESSION_STATE_READY: {
         CHECK(session_ != XR_NULL_HANDLE);
         XrSessionBeginInfo session_begin_info = {XR_TYPE_SESSION_BEGIN_INFO};
         session_begin_info.primaryViewConfigurationType = options_->Parsed.view_config_type;
         CHECK_XRCMD(xrBeginSession(session_, &session_begin_info));
         session_running_ = true;
         break;
      }
      case XR_SESSION_STATE_STOPPING: {
         CHECK(session_ != XR_NULL_HANDLE);
         session_running_ = false;
         CHECK_XRCMD(xrEndSession(session_))
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
      CHECK(instance_ == XR_NULL_HANDLE);

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

      CHECK_XRCMD(xrCreateInstance(&create_info, &instance_));
   }

   void OpenXrProgram::CreateInstance() {
      ProgramLogger::Get()->LogLayersAndExtensions();
      CreateInstanceInternal();
      ProgramLogger::Get()->LogInstanceInfo();
   }

   XrEnvironmentBlendMode OpenXrProgram::GetPreferredBlendMode() const {
      uint32_t count;
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(
          instance_, system_id_, options_->Parsed.view_config_type, 0, &count, nullptr));
      CHECK(count > 0);

      std::vector<XrEnvironmentBlendMode> blend_modes(count);
      CHECK_XRCMD(xrEnumerateEnvironmentBlendModes(
          instance_, system_id_, options_->Parsed.view_config_type, count, &count,
          blend_modes.data()));

      for (const auto &blend_mode : blend_modes) {
         if (kAcceptableBlendModes.count(blend_mode))
            return blend_mode;
      }

      THROW("No acceptable blend mode returned from the xrEnumerateEnvironmentBlendModes");
   }

   void OpenXrProgram::InitSystem() {
      CHECK(instance_ != XR_NULL_HANDLE);
      CHECK(system_id_ == XR_NULL_SYSTEM_ID);

      XrSystemGetInfo system_info = {XR_TYPE_SYSTEM_GET_INFO};
      system_info.formFactor = options_->Parsed.form_factor;
      CHECK_XRCMD(xrGetSystem(instance_, &system_info, &system_id_));

      Log::Write(
          Log::Level::Verbose, Fmt("Using system %d for form factor %s", system_id_,
                                   to_string(options_->Parsed.form_factor)));
      CHECK(instance_ != XR_NULL_HANDLE);
      CHECK(system_id_ != XR_NULL_SYSTEM_ID);
   }

   void OpenXrProgram::InitDevice() {
      ProgramLogger::Get()->LogViewConfigurations();

      // The graphics API can initialize the graphics device now that the systemId and
      // instance handle are available.
      graphics_plugin_->InitializeDevice(instance_, system_id_);
   }

   void OpenXrProgram::CreateVisualizedSpaces() {
      CHECK(session_ != XR_NULL_HANDLE);

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
            Log::Write(
                Log::Level::Warning,
                Fmt("Failed to create reference space for a tile with error %d", res));
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
      //         Log::Write(
      //             Log::Level::Warning, Fmt("Failed to create reference space %s with error %d",
      //                                      visualized_space.c_str(), res));
      //     }
      // }
   }

   void OpenXrProgram::InitSession() {
      CHECK(instance_ != XR_NULL_HANDLE);
      CHECK(session_ == XR_NULL_HANDLE);

      {
         Log::Write(Log::Level::Verbose, Fmt("Creating session..."));

         XrSessionCreateInfo create_info = {XR_TYPE_SESSION_CREATE_INFO};
         create_info.next = graphics_plugin_->GetGraphicsBinding();
         create_info.systemId = system_id_;
         CHECK_XRCMD(xrCreateSession(instance_, &create_info, &session_));
      }

      ProgramLogger::Get()->LogReferenceSpaces();
      GET(InputActionsHandler)->InitInputActions();
      CreateVisualizedSpaces();

      {
         XrReferenceSpaceCreateInfo reference_space_create_info =
             GetXrReferenceSpaceCreateInfo(options_->app_space);
         CHECK_XRCMD(xrCreateReferenceSpace(session_, &reference_space_create_info, &app_space_));
      }
   }

   void OpenXrProgram::CreateSwapchains() {
      CHECK(session_ != XR_NULL_HANDLE);
      CHECK(swapchains_.empty());
      CHECK(config_views_.empty());

      // Read graphics properties for preferred swapchain length and logging.
      XrSystemProperties system_properties = {XR_TYPE_SYSTEM_PROPERTIES};
      CHECK_XRCMD(xrGetSystemProperties(instance_, system_id_, &system_properties));

      // Log system properties.
      Log::Write(
          Log::Level::Info, Fmt("System Properties: Name=%s VendorId=%d",
                                system_properties.systemName, system_properties.vendorId));
      Log::Write(
          Log::Level::Info, Fmt("System Graphics Properties: MaxWidth=%d MaxHeight=%d MaxLayers=%d",
                                system_properties.graphicsProperties.maxSwapchainImageWidth,
                                system_properties.graphicsProperties.maxSwapchainImageHeight,
                                system_properties.graphicsProperties.maxLayerCount));
      Log::Write(
          Log::Level::Info,
          Fmt("System Tracking Properties: OrientationTracking=%s PositionTracking=%s",
              system_properties.trackingProperties.orientationTracking == XR_TRUE ? "True"
                                                                                  : "False",
              system_properties.trackingProperties.positionTracking == XR_TRUE ? "True" : "False"));

      // Note: No other view configurations exist at the time this code was written. If
      // this condition is not met, the project will need to be audited to see how support
      // should be added.
      CHECK_MSG(
          options_->Parsed.view_config_type == XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
          "Unsupported view configuration type");

      // Query and cache view configuration views.
      uint32_t view_count;
      CHECK_XRCMD(xrEnumerateViewConfigurationViews(
          instance_, system_id_, options_->Parsed.view_config_type, 0, &view_count, nullptr));
      config_views_.resize(view_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
      CHECK_XRCMD(xrEnumerateViewConfigurationViews(
          instance_, system_id_, options_->Parsed.view_config_type, view_count, &view_count,
          config_views_.data()));

      // Create and cache view buffer for xrLocateViews later.
      views_.resize(view_count, {XR_TYPE_VIEW});

      // Create the swapchain and get the images.
      if (view_count > 0) {
         // Select a swapchain format.
         uint32_t swapchain_format_count;
         CHECK_XRCMD(xrEnumerateSwapchainFormats(session_, 0, &swapchain_format_count, nullptr));
         std::vector<int64_t> swapchain_formats(swapchain_format_count);
         CHECK_XRCMD(xrEnumerateSwapchainFormats(
             session_, (uint32_t)swapchain_formats.size(), &swapchain_format_count,
             swapchain_formats.data()));
         CHECK(swapchain_format_count == swapchain_formats.size());
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
            Log::Write(
                Log::Level::Verbose,
                Fmt("Swapchain Formats: %s", swapchain_formats_string.c_str()));
         }

         // Create a swapchain for each view.
         for (uint32_t i = 0; i < view_count; i++) {
            const XrViewConfigurationView &vp = config_views_[i];
            Log::Write(
                Log::Level::Info,
                Fmt("Creating swapchain for view %d with dimensions Width=%d Height=%d "
                    "SampleCount=%d",
                    i, vp.recommendedImageRectWidth, vp.recommendedImageRectHeight,
                    vp.recommendedSwapchainSampleCount));

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
            CHECK_XRCMD(xrCreateSwapchain(session_, &swapchain_create_info, &swapchain.handle));

            swapchains_.push_back(swapchain);

            uint32_t image_count;
            CHECK_XRCMD(xrEnumerateSwapchainImages(swapchain.handle, 0, &image_count, nullptr));
            // XXX This should really just return XrSwapchainImageBaseHeader*
            std::vector<XrSwapchainImageBaseHeader *> swapchain_images =
                graphics_plugin_->AllocateSwapchainImageStructs(image_count, swapchain_create_info);
            CHECK_XRCMD(xrEnumerateSwapchainImages(
                swapchain.handle, image_count, &image_count, swapchain_images[0]));

            swapchain_images_.insert(std::make_pair(swapchain.handle, std::move(swapchain_images)));
         }
      }
   }
}