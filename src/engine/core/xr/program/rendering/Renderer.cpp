#include "Pch.h"
#include "Renderer.h"
#include "engine/core/xr/program/OpenXrProgram.h"
#include "engine/core/xr/common/Common.h"
#include "engine/core/xr/graphics_plugin/OpenGLESGraphicsPlugin.h"
#include "engine/OptionsManager.h"
#include "matter/Cube.h"

namespace nar {
  void Renderer::RenderFrame() {
    auto session = OpenXrProgram::Get()->session();

    CHECK(session != XR_NULL_HANDLE);

    XrFrameWaitInfo frame_wait_info = {XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frame_state = {XR_TYPE_FRAME_STATE};
    CHECK_XRCMD(xrWaitFrame(session, &frame_wait_info, &frame_state));

    XrFrameBeginInfo frame_begin_info = {XR_TYPE_FRAME_BEGIN_INFO};
    CHECK_XRCMD(xrBeginFrame(session, &frame_begin_info));

    std::vector<XrCompositionLayerBaseHeader *> layers;
    XrCompositionLayerProjection layer = {XR_TYPE_COMPOSITION_LAYER_PROJECTION};
    std::vector<XrCompositionLayerProjectionView> projection_layer_views;

    if (frame_state.shouldRender == XR_TRUE) {
      if (RenderLayer(frame_state.predictedDisplayTime, projection_layer_views, layer))
        layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&layer));
    }

    XrFrameEndInfo frame_end_info = {XR_TYPE_FRAME_END_INFO};
    frame_end_info.displayTime = frame_state.predictedDisplayTime;
    frame_end_info.environmentBlendMode =
        OptionsManager::Get()->options()->Parsed.environment_blend_mode;
    frame_end_info.layerCount = (uint32_t)layers.size();
    frame_end_info.layers = layers.data();
    CHECK_XRCMD(xrEndFrame(session, &frame_end_info));
  }

  bool Renderer::RenderLayer(
      XrTime predicted_display_time,
      std::vector<XrCompositionLayerProjectionView> &projection_layer_views,
      XrCompositionLayerProjection &layer) {
    XrResult res;

    XrViewState view_state = {XR_TYPE_VIEW_STATE};
    uint32_t view_capacity_input = (uint32_t)OpenXrProgram::Get()->views().size();
    uint32_t view_count_output;

    XrViewLocateInfo view_locate_info = {XR_TYPE_VIEW_LOCATE_INFO};
    view_locate_info.viewConfigurationType =
        OptionsManager::Get()->options()->Parsed.view_config_type;
    view_locate_info.displayTime = predicted_display_time;
    view_locate_info.space = OpenXrProgram::Get()->app_space();

    std::vector<XrView> views = OpenXrProgram::Get()->views();

    res = xrLocateViews(
        OpenXrProgram::Get()->session(), &view_locate_info, &view_state, view_capacity_input,
        &view_count_output, views.data());
    CHECK_XRRESULT(res, "xrLocateViews");

    OpenXrProgram::Get()->set_views(views);

    if ((view_state.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
        (view_state.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
      return false; // There is no valid tracking poses for the views.
    }

    CHECK(view_count_output == view_capacity_input);
    CHECK(view_count_output == OpenXrProgram::Get()->config_views().size());
    CHECK(view_count_output == OpenXrProgram::Get()->swapchains().size());

    projection_layer_views.resize(view_count_output);

    // For each locatable space that we want to visualize, render a 25cm cube.
    std::vector<Cube> cubes;

    for (XrSpace visualized_space : OpenXrProgram::Get()->visualized_spaces()) {
      XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
      res = xrLocateSpace(
          visualized_space, OpenXrProgram::Get()->app_space(), predicted_display_time,
          &space_location);
      CHECK_XRRESULT(res, "xrLocateSpace");

      if (XR_UNQUALIFIED_SUCCESS(res)) {
        if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
            (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
          cubes.push_back(Cube{space_location.pose, {0.25f, 0.25f, 0.25f}});
        }
      }
      else {
        Log::Write(
            Log::Level::Verbose,
            Fmt("Unable to locate a visualized reference space in app space: %d", res));
      }
    }

    // Render a 10cm cube scaled by grab_action for each hand. Note renderHand will only
    // be true when the application has focus.
    for (auto hand : {Side::kLeft, Side::kRight}) {
      XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
      res = xrLocateSpace(
          OpenXrProgram::Get()->input().hand_space[hand], OpenXrProgram::Get()->app_space(),
          predicted_display_time, &space_location);
      CHECK_XRRESULT(res, "xrLocateSpace");

      if (XR_UNQUALIFIED_SUCCESS(res)) {
        if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
            (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
          float scale = 0.1f * OpenXrProgram::Get()->input().hand_scale[hand];
          cubes.push_back(Cube{space_location.pose, {scale, scale, scale}});
        }
      }
      else {
        // Tracking loss is expected when the hand is not active so only log a
        // message if the hand is active.
        if (OpenXrProgram::Get()->input().hand_active[hand] == XR_TRUE) {
          const char *hand_name[] = {"left", "right"};
          Log::Write(
              Log::Level::Verbose,
              Fmt("Unable to locate %s hand action space in app space: %d", hand_name[hand], res));
        }
      }
    }

    // Render view to the appropriate part of the swapchain image.
    for (uint32_t i = 0; i < view_count_output; i++) {
      // Each view has a separate swapchain which is acquired, rendered to, and
      // released.
      const Swapchain view_swapchain = OpenXrProgram::Get()->swapchains()[i];

      XrSwapchainImageAcquireInfo acquire_info = {XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};

      uint32_t swapchain_image_index;
      CHECK_XRCMD(
          xrAcquireSwapchainImage(view_swapchain.handle, &acquire_info, &swapchain_image_index));

      XrSwapchainImageWaitInfo wait_info = {XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
      wait_info.timeout = XR_INFINITE_DURATION;
      CHECK_XRCMD(xrWaitSwapchainImage(view_swapchain.handle, &wait_info));

      projection_layer_views[i] = {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW};
      projection_layer_views[i].pose = OpenXrProgram::Get()->views()[i].pose;
      projection_layer_views[i].fov = OpenXrProgram::Get()->views()[i].fov;
      projection_layer_views[i].subImage.swapchain = view_swapchain.handle;
      projection_layer_views[i].subImage.imageRect.offset = {0, 0};
      projection_layer_views[i].subImage.imageRect.extent = {
          view_swapchain.width, view_swapchain.height};

      const XrSwapchainImageBaseHeader *const swapchain_image =
          OpenXrProgram::Get()->swapchain_images()[view_swapchain.handle][swapchain_image_index];
      OpenGLESGraphicsPlugin::Get()->RenderView(
          projection_layer_views[i], swapchain_image,
          OpenXrProgram::Get()->color_swapchain_format(), cubes);

      XrSwapchainImageReleaseInfo release_info = {XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
      CHECK_XRCMD(xrReleaseSwapchainImage(view_swapchain.handle, &release_info));
    }

    layer.space = OpenXrProgram::Get()->app_space();
    layer.layerFlags = OptionsManager::Get()->options()->Parsed.environment_blend_mode ==
                               XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND
                           ? XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
                                 XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT
                           : 0;
    layer.viewCount = (uint32_t)projection_layer_views.size();
    layer.views = projection_layer_views.data();

    return true;
  }
}