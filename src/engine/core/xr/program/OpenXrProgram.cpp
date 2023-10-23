#include "Pch.h"
#include "OpenXrProgram.h"
#include "engine/OptionsManager.h"
#include "engine/core/xr/common/Common.h"
#include "engine/core/xr/options/Options.h"
#include "engine/core/xr/graphics_plugin/OpenGLESGraphicsPlugin.h"
#include "engine/core/xr/platform_plugin/AndroidPlatformPlugin.h"
#include "func_GetXrReferenceSpaceCreateInfo.h"
#include "func_GetXrReferenceSpaceCreateInfoTiles.h"
#include "rendering/Renderer.h"
#include "logging/ProgramLogger.h"
#include "engine/core/GameLoop.h"
#include <common/xr_linear.h>
#include <array>
#include <cmath>
#include <set>

#if !defined(XR_USE_PLATFORM_WIN32)
#define strcpy_s(dest, source) strncpy((dest), (source), sizeof(dest))
#endif

namespace nar {
  OpenXrProgram::OpenXrProgram()
      : options_(OptionsManager::Get()->options()),
        platform_plugin_(AndroidPlatformPlugin::Get()),
        graphics_plugin_(OpenGLESGraphicsPlugin::Get()),
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

  void OpenXrProgram::Initialize() {
    CreateInstance();
    InitializeSystem();
    InitializeDevice();
    InitializeSession();
    CreateSwapchains();
  }

  void OpenXrProgram::RenderFrame() {
    Renderer::Get()->RenderFrame();
  }

  void OpenXrProgram::CreateInstanceInternal() {
    CHECK(instance_ == XR_NULL_HANDLE);

    // Create union of extensions required by platform and graphics plugins.
    std::vector<const char *> extensions;

    // Transform platform and graphics extension std::strings to C strings.
    const std::vector<std::string> platform_extensions = platform_plugin_->GetInstanceExtensions();
    std::transform(
        platform_extensions.begin(), platform_extensions.end(), std::back_inserter(extensions),
        [](const std::string &ext) { return ext.c_str(); });
    const std::vector<std::string> graphics_extensions = graphics_plugin_->GetInstanceExtensions();
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

  void OpenXrProgram::InitializeSystem() {
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

  void OpenXrProgram::InitializeDevice() {
    ProgramLogger::Get()->LogViewConfigurations();

    // The graphics API can initialize the graphics device now that the systemId and
    // instance handle are available.
    graphics_plugin_->InitializeDevice(instance_, system_id_);
  }

  void OpenXrProgram::InitializeActions() {
    // Create an action set.
    {
      XrActionSetCreateInfo action_set_info = {XR_TYPE_ACTION_SET_CREATE_INFO};
      strcpy_s(action_set_info.actionSetName, "gameplay");
      strcpy_s(action_set_info.localizedActionSetName, "Gameplay");
      action_set_info.priority = 0;
      CHECK_XRCMD(xrCreateActionSet(instance_, &action_set_info, &input_.action_set));
    }

    // Get the XrPath for the left and right hands - we will use them as subaction
    // paths.
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/left", &input_.hand_subaction_path[Side::kLeft]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/right", &input_.hand_subaction_path[Side::kRight]));

    // Create actions.
    {
      // Create an input action for grabbing objects with the left and right hands.
      XrActionCreateInfo action_info = {XR_TYPE_ACTION_CREATE_INFO};
      action_info.actionType = XR_ACTION_TYPE_FLOAT_INPUT;
      strcpy_s(action_info.actionName, "grab_object");
      strcpy_s(action_info.localizedActionName, "Grab Object");
      action_info.countSubactionPaths = uint32_t(input_.hand_subaction_path.size());
      action_info.subactionPaths = input_.hand_subaction_path.data();
      CHECK_XRCMD(xrCreateAction(input_.action_set, &action_info, &input_.grab_action));

      // Create an input action getting the left and right hand poses.
      action_info.actionType = XR_ACTION_TYPE_POSE_INPUT;
      strcpy_s(action_info.actionName, "hand_pose");
      strcpy_s(action_info.localizedActionName, "Hand Pose");
      action_info.countSubactionPaths = uint32_t(input_.hand_subaction_path.size());
      action_info.subactionPaths = input_.hand_subaction_path.data();
      CHECK_XRCMD(xrCreateAction(input_.action_set, &action_info, &input_.pose_action));

      // Create output actions for vibrating the left and right controller.
      action_info.actionType = XR_ACTION_TYPE_VIBRATION_OUTPUT;
      strcpy_s(action_info.actionName, "vibrate_hand");
      strcpy_s(action_info.localizedActionName, "Vibrate Hand");
      action_info.countSubactionPaths = uint32_t(input_.hand_subaction_path.size());
      action_info.subactionPaths = input_.hand_subaction_path.data();
      CHECK_XRCMD(xrCreateAction(input_.action_set, &action_info, &input_.vibrate_action));

      // Create input actions for quitting the session using the left and right
      // controller. Since it doesn't matter which hand did this, we do not specify
      // subaction paths for it. We will just suggest bindings for both hands, where
      // possible.
      action_info.actionType = XR_ACTION_TYPE_BOOLEAN_INPUT;
      strcpy_s(action_info.actionName, "quit_session");
      strcpy_s(action_info.localizedActionName, "Quit Session");
      action_info.countSubactionPaths = 0;
      action_info.subactionPaths = nullptr;
      CHECK_XRCMD(xrCreateAction(input_.action_set, &action_info, &input_.quit_action));
    }

    std::array<XrPath, Side::kCount> select_path;
    std::array<XrPath, Side::kCount> squeeze_value_path;
    std::array<XrPath, Side::kCount> squeeze_force_path;
    std::array<XrPath, Side::kCount> squeeze_click_path;
    std::array<XrPath, Side::kCount> pose_path;
    std::array<XrPath, Side::kCount> haptic_path;
    std::array<XrPath, Side::kCount> menu_click_path;
    std::array<XrPath, Side::kCount> b_click_path;
    std::array<XrPath, Side::kCount> trigger_value_path;
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/left/input/select/click", &select_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/select/click", &select_path[Side::kRight]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/left/input/squeeze/value", &squeeze_value_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/squeeze/value", &squeeze_value_path[Side::kRight]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/left/input/squeeze/force", &squeeze_force_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/squeeze/force", &squeeze_force_path[Side::kRight]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/left/input/squeeze/click", &squeeze_click_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/squeeze/click", &squeeze_click_path[Side::kRight]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/left/input/grip/pose", &pose_path[Side::kLeft]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/right/input/grip/pose", &pose_path[Side::kRight]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/left/output/haptic", &haptic_path[Side::kLeft]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/right/output/haptic", &haptic_path[Side::kRight]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/left/input/menu/click", &menu_click_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/menu/click", &menu_click_path[Side::kRight]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/left/input/b/click", &b_click_path[Side::kLeft]));
    CHECK_XRCMD(
        xrStringToPath(instance_, "/user/hand/right/input/b/click", &b_click_path[Side::kRight]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/left/input/trigger/value", &trigger_value_path[Side::kLeft]));
    CHECK_XRCMD(xrStringToPath(
        instance_, "/user/hand/right/input/trigger/value", &trigger_value_path[Side::kRight]));
    // Suggest bindings for KHR Simple.
    {
      XrPath khr_simple_interaction_profile_path;
      CHECK_XRCMD(xrStringToPath(
          instance_, "/interaction_profiles/khr/simple_controller",
          &khr_simple_interaction_profile_path));
      std::vector<XrActionSuggestedBinding> bindings = {
          {// Fall back to a click input for the grab action.
           {input_.grab_action, select_path[Side::kLeft]},
           {input_.grab_action, select_path[Side::kRight]},
           {input_.pose_action, pose_path[Side::kLeft]},
           {input_.pose_action, pose_path[Side::kRight]},
           {input_.quit_action, menu_click_path[Side::kLeft]},
           {input_.quit_action, menu_click_path[Side::kRight]},
           {input_.vibrate_action, haptic_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kRight]}}};
      XrInteractionProfileSuggestedBinding suggested_bindings = {
          XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggested_bindings.interactionProfile = khr_simple_interaction_profile_path;
      suggested_bindings.suggestedBindings = bindings.data();
      suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
      CHECK_XRCMD(xrSuggestInteractionProfileBindings(instance_, &suggested_bindings));
    }
    // Suggest bindings for the Oculus Touch.
    {
      XrPath oculus_touch_interaction_profile_path;
      CHECK_XRCMD(xrStringToPath(
          instance_, "/interaction_profiles/oculus/touch_controller",
          &oculus_touch_interaction_profile_path));
      std::vector<XrActionSuggestedBinding> bindings = {
          {{input_.grab_action, squeeze_value_path[Side::kLeft]},
           {input_.grab_action, squeeze_value_path[Side::kRight]},
           {input_.pose_action, pose_path[Side::kLeft]},
           {input_.pose_action, pose_path[Side::kRight]},
           {input_.quit_action, menu_click_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kRight]}}};
      XrInteractionProfileSuggestedBinding suggested_bindings = {
          XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggested_bindings.interactionProfile = oculus_touch_interaction_profile_path;
      suggested_bindings.suggestedBindings = bindings.data();
      suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
      CHECK_XRCMD(xrSuggestInteractionProfileBindings(instance_, &suggested_bindings));
    }
    // Suggest bindings for the Vive Controller.
    {
      XrPath vive_controller_interaction_profile_path;
      CHECK_XRCMD(xrStringToPath(
          instance_, "/interaction_profiles/htc/vive_controller",
          &vive_controller_interaction_profile_path));
      std::vector<XrActionSuggestedBinding> bindings = {
          {{input_.grab_action, trigger_value_path[Side::kLeft]},
           {input_.grab_action, trigger_value_path[Side::kRight]},
           {input_.pose_action, pose_path[Side::kLeft]},
           {input_.pose_action, pose_path[Side::kRight]},
           {input_.quit_action, menu_click_path[Side::kLeft]},
           {input_.quit_action, menu_click_path[Side::kRight]},
           {input_.vibrate_action, haptic_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kRight]}}};
      XrInteractionProfileSuggestedBinding suggested_bindings = {
          XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggested_bindings.interactionProfile = vive_controller_interaction_profile_path;
      suggested_bindings.suggestedBindings = bindings.data();
      suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
      CHECK_XRCMD(xrSuggestInteractionProfileBindings(instance_, &suggested_bindings));
    }

    // Suggest bindings for the Valve Index Controller.
    {
      XrPath index_controller_interaction_profile_path;
      CHECK_XRCMD(xrStringToPath(
          instance_, "/interaction_profiles/valve/index_controller",
          &index_controller_interaction_profile_path));
      std::vector<XrActionSuggestedBinding> bindings = {
          {{input_.grab_action, squeeze_force_path[Side::kLeft]},
           {input_.grab_action, squeeze_force_path[Side::kRight]},
           {input_.pose_action, pose_path[Side::kLeft]},
           {input_.pose_action, pose_path[Side::kRight]},
           {input_.quit_action, b_click_path[Side::kLeft]},
           {input_.quit_action, b_click_path[Side::kRight]},
           {input_.vibrate_action, haptic_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kRight]}}};
      XrInteractionProfileSuggestedBinding suggested_bindings = {
          XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggested_bindings.interactionProfile = index_controller_interaction_profile_path;
      suggested_bindings.suggestedBindings = bindings.data();
      suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
      CHECK_XRCMD(xrSuggestInteractionProfileBindings(instance_, &suggested_bindings));
    }

    // Suggest bindings for the Microsoft Mixed Reality Motion Controller.
    {
      XrPath microsoft_mixed_reality_interaction_profile_path;
      CHECK_XRCMD(xrStringToPath(
          instance_, "/interaction_profiles/microsoft/motion_controller",
          &microsoft_mixed_reality_interaction_profile_path));
      std::vector<XrActionSuggestedBinding> bindings = {
          {{input_.grab_action, squeeze_click_path[Side::kLeft]},
           {input_.grab_action, squeeze_click_path[Side::kRight]},
           {input_.pose_action, pose_path[Side::kLeft]},
           {input_.pose_action, pose_path[Side::kRight]},
           {input_.quit_action, menu_click_path[Side::kLeft]},
           {input_.quit_action, menu_click_path[Side::kRight]},
           {input_.vibrate_action, haptic_path[Side::kLeft]},
           {input_.vibrate_action, haptic_path[Side::kRight]}}};
      XrInteractionProfileSuggestedBinding suggested_bindings = {
          XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
      suggested_bindings.interactionProfile = microsoft_mixed_reality_interaction_profile_path;
      suggested_bindings.suggestedBindings = bindings.data();
      suggested_bindings.countSuggestedBindings = (uint32_t)bindings.size();
      CHECK_XRCMD(xrSuggestInteractionProfileBindings(instance_, &suggested_bindings));
    }
    XrActionSpaceCreateInfo action_space_info = {XR_TYPE_ACTION_SPACE_CREATE_INFO};
    action_space_info.action = input_.pose_action;
    action_space_info.poseInActionSpace.orientation.w = 1.f;
    action_space_info.subactionPath = input_.hand_subaction_path[Side::kLeft];
    CHECK_XRCMD(xrCreateActionSpace(session_, &action_space_info, &input_.hand_space[Side::kLeft]));
    action_space_info.subactionPath = input_.hand_subaction_path[Side::kRight];
    CHECK_XRCMD(
        xrCreateActionSpace(session_, &action_space_info, &input_.hand_space[Side::kRight]));

    XrSessionActionSetsAttachInfo attach_info = {XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
    attach_info.countActionSets = 1;
    attach_info.actionSets = &input_.action_set;
    CHECK_XRCMD(xrAttachSessionActionSets(session_, &attach_info));
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

  void OpenXrProgram::InitializeSession() {
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
    InitializeActions();
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
            system_properties.trackingProperties.orientationTracking == XR_TRUE ? "True" : "False",
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
            Log::Level::Verbose, Fmt("Swapchain Formats: %s", swapchain_formats_string.c_str()));
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
        swapchain_create_info.sampleCount = graphics_plugin_->GetSupportedSwapchainSampleCount(vp);
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

  // Return event if one is available, otherwise return null.
  const XrEventDataBaseHeader *OpenXrProgram::TryReadNextEvent() {
    // It is sufficient to clear the just the XrEventDataBuffer header to
    // XR_TYPE_EVENT_DATA_BUFFER
    XrEventDataBaseHeader *base_header =
        reinterpret_cast<XrEventDataBaseHeader *>(&event_data_buffer_);
    *base_header = {XR_TYPE_EVENT_DATA_BUFFER};
    const XrResult xr = xrPollEvent(instance_, &event_data_buffer_);

    if (xr == XR_SUCCESS) {
      if (base_header->type == XR_TYPE_EVENT_DATA_EVENTS_LOST) {
        const XrEventDataEventsLost *const eventsLost =
            reinterpret_cast<const XrEventDataEventsLost *>(base_header);
        Log::Write(Log::Level::Warning, Fmt("%d events lost", eventsLost->lostEventCount));
      }

      return base_header;
    }

    if (xr == XR_EVENT_UNAVAILABLE)
      return nullptr;

    THROW_XR(xr, "xrPollEvent");
  }

  void OpenXrProgram::PollEvents() {
    GameLoop::Get()->set_exit_render_loop(false);
    GameLoop::Get()->set_request_restart(false);

    // Process all pending messages.
    while (const XrEventDataBaseHeader *event = TryReadNextEvent()) {
      switch (event->type) {
      case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
        const auto &instance_loss_pending =
            *reinterpret_cast<const XrEventDataInstanceLossPending *>(event);
        Log::Write(
            Log::Level::Warning,
            Fmt("XrEventDataInstanceLossPending by %lld", instance_loss_pending.lossTime));

        GameLoop::Get()->set_exit_render_loop(true);
        GameLoop::Get()->set_request_restart(true);
        return;
      }
      case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
        auto session_state_changed_event =
            *reinterpret_cast<const XrEventDataSessionStateChanged *>(event);
        HandleSessionStateChangedEvent(session_state_changed_event);
        break;
      }
      case XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED:
        ProgramLogger::Get()->LogActionSourceName(input_.grab_action, "Grab");
        ProgramLogger::Get()->LogActionSourceName(input_.quit_action, "Quit");
        ProgramLogger::Get()->LogActionSourceName(input_.pose_action, "Pose");
        ProgramLogger::Get()->LogActionSourceName(input_.vibrate_action, "Vibrate");
        break;
      case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING:
      default: {
        Log::Write(Log::Level::Verbose, Fmt("Ignoring event type %d", event->type));
        break;
      }
      }
    }
  }

  void OpenXrProgram::HandleSessionStateChangedEvent(
      const XrEventDataSessionStateChanged &state_changed_event) {
    const XrSessionState old_state = session_state_;
    session_state_ = state_changed_event.state;

    Log::Write(
        Log::Level::Info, Fmt("XrEventDataSessionStateChanged: state %s->%s session=%lld time=%lld",
                              to_string(old_state), to_string(session_state_),
                              state_changed_event.session, state_changed_event.time));

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

    void OpenXrProgram::PollActions() {
      input_.hand_active = {XR_FALSE, XR_FALSE};

      // Sync actions
      const XrActiveActionSet active_action_set = {input_.action_set, XR_NULL_PATH};
      XrActionsSyncInfo sync_info = {XR_TYPE_ACTIONS_SYNC_INFO};
      sync_info.countActiveActionSets = 1;
      sync_info.activeActionSets = &active_action_set;
      CHECK_XRCMD(xrSyncActions(session_, &sync_info));

      // Get pose and grab action state and start haptic vibrate when hand is 90%
      // squeezed.
      for (auto hand : {Side::kLeft, Side::kRight}) {
        XrActionStateGetInfo get_info = {XR_TYPE_ACTION_STATE_GET_INFO};
        get_info.action = input_.grab_action;
        get_info.subactionPath = input_.hand_subaction_path[hand];

        XrActionStateFloat grab_value = {XR_TYPE_ACTION_STATE_FLOAT};
        CHECK_XRCMD(xrGetActionStateFloat(session_, &get_info, &grab_value));

        if (grab_value.isActive == XR_TRUE) {
          // Scale the rendered hand by 1.0f (open) to 0.5f (fully squeezed).
          input_.hand_scale[hand] = 1.0f - 0.5f * grab_value.currentState;

          if (grab_value.currentState > 0.9f) {
            XrHapticVibration vibration = {XR_TYPE_HAPTIC_VIBRATION};
            vibration.amplitude = 0.5;
            vibration.duration = XR_MIN_HAPTIC_DURATION;
            vibration.frequency = XR_FREQUENCY_UNSPECIFIED;

            XrHapticActionInfo haptic_action_info = {XR_TYPE_HAPTIC_ACTION_INFO};
            haptic_action_info.action = input_.vibrate_action;
            haptic_action_info.subactionPath = input_.hand_subaction_path[hand];
            CHECK_XRCMD(xrApplyHapticFeedback(
                session_, &haptic_action_info, (XrHapticBaseHeader *)&vibration));
          }
        }

        get_info.action = input_.pose_action;
        XrActionStatePose pose_state = {XR_TYPE_ACTION_STATE_POSE};
        CHECK_XRCMD(xrGetActionStatePose(session_, &get_info, &pose_state));
        input_.hand_active[hand] = pose_state.isActive;
      }

      // There were no subaction paths specified for the quit action, because we don't
      // care which hand did it.
      XrActionStateGetInfo get_info = {
          XR_TYPE_ACTION_STATE_GET_INFO, nullptr, input_.quit_action, XR_NULL_PATH};
      XrActionStateBoolean quit_value = {XR_TYPE_ACTION_STATE_BOOLEAN};
      CHECK_XRCMD(xrGetActionStateBoolean(session_, &get_info, &quit_value));

      if ((quit_value.isActive == XR_TRUE) && (quit_value.changedSinceLastSync == XR_TRUE) &&
          (quit_value.currentState == XR_TRUE)) {
        CHECK_XRCMD(xrRequestExitSession(session_));
      }
    }
  }