/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "RendererView.h"
#include "CubeLayerView.h"
#include "FacelockedLayerView.h"
#include "core/model/OpenXrProgram.h"
#include "core/model/OptionsManager.h"
#include "core/view/GraphicsGLView.h"
#include "matter/model/Cube.h"

namespace nar {
    void RendererView::RenderFrame(
        std::function<void(XrMatrix4x4f)> gl_render_code, Point3F player_translation,
        std::function<void(XrMatrix4x4f)> gl_facelocked_render_code) {
        auto session = OpenXrProgram::Get()->session();

        XrFrameWaitInfo frame_wait_info = {XR_TYPE_FRAME_WAIT_INFO};
        XrFrameState frame_state = {XR_TYPE_FRAME_STATE};
        xrWaitFrame(session, &frame_wait_info, &frame_state);

        XrFrameBeginInfo frame_begin_info = {XR_TYPE_FRAME_BEGIN_INFO};
        xrBeginFrame(session, &frame_begin_info);

        std::vector<XrCompositionLayerBaseHeader *> layers;
        std::vector<XrCompositionLayerProjectionView> projection_layer_views;

        predicted_display_time_ = frame_state.predictedDisplayTime;

        if (frame_state.shouldRender == XR_TRUE) {
            CubeLayerView cube_layer;
            FacelockedLayerView facelocked_layer;
            if (cube_layer.Render(
                    frame_state.predictedDisplayTime, projection_layer_views, gl_render_code,
                    player_translation))
                layers.push_back(
                    reinterpret_cast<XrCompositionLayerBaseHeader *>(cube_layer.layer().get()));
            if (facelocked_layer.Render(
                    frame_state.predictedDisplayTime, projection_layer_views,
                    gl_facelocked_render_code))
                layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(
                    facelocked_layer.layer().get()));
        }

        XrFrameEndInfo frame_end_info = {XR_TYPE_FRAME_END_INFO};
        frame_end_info.displayTime = frame_state.predictedDisplayTime;
        frame_end_info.environmentBlendMode =
            OptionsManager::Get()->options()->Parsed.environment_blend_mode;
        frame_end_info.layerCount = (uint32_t)layers.size();
        frame_end_info.layers = layers.data();
        xrEndFrame(session, &frame_end_info);
    }
}
