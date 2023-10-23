#include "Pch.h"
#include "Renderer.h"
#include "engine/core/xr/program/OpenXrProgram.h"
#include "engine/core/xr/common/Common.h"
#include "engine/core/xr/graphics_plugin/OpenGLESGraphicsPlugin.h"
#include "engine/OptionsManager.h"
#include "matter/Cube.h"
#include "layers/CubeLayer.h"

namespace nar {
  void Renderer::RenderFrame(std::vector<Cube> cubes_data) {
    auto session = OpenXrProgram::Get()->session();

    CHECK(session != XR_NULL_HANDLE);

    XrFrameWaitInfo frame_wait_info = {XR_TYPE_FRAME_WAIT_INFO};
    XrFrameState frame_state = {XR_TYPE_FRAME_STATE};
    CHECK_XRCMD(xrWaitFrame(session, &frame_wait_info, &frame_state));

    XrFrameBeginInfo frame_begin_info = {XR_TYPE_FRAME_BEGIN_INFO};
    CHECK_XRCMD(xrBeginFrame(session, &frame_begin_info));

    std::vector<XrCompositionLayerBaseHeader *> layers;
    std::vector<XrCompositionLayerProjectionView> projection_layer_views;

    predicted_display_time_ = frame_state.predictedDisplayTime;

    if (frame_state.shouldRender == XR_TRUE) {
      CubeLayer cube_layer;
      if (cube_layer.Render(frame_state.predictedDisplayTime, projection_layer_views, cubes_data))
        layers.push_back(
            reinterpret_cast<XrCompositionLayerBaseHeader *>(cube_layer.layer().get()));
    }

    XrFrameEndInfo frame_end_info = {XR_TYPE_FRAME_END_INFO};
    frame_end_info.displayTime = frame_state.predictedDisplayTime;
    frame_end_info.environmentBlendMode =
        OptionsManager::Get()->options()->Parsed.environment_blend_mode;
    frame_end_info.layerCount = (uint32_t)layers.size();
    frame_end_info.layers = layers.data();
    CHECK_XRCMD(xrEndFrame(session, &frame_end_info));
  }
}