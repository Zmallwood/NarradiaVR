/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_Renderer.h"
#include "system/system_OpenXrProgram.h"
#include "view/system_GraphicsGL.h"
#include "system/system_OptionsManager.h"
#include "model/matter/struct_Cube.h"
#include "layers/system_CubeLayer.h"

namespace nar {
   void Renderer::RenderFrame(std::vector<Cube> cubes_data) {
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
         CubeLayer cube_layer;
         if (cube_layer.Render(
                 frame_state.predictedDisplayTime, projection_layer_views, cubes_data))
            layers.push_back(
                reinterpret_cast<XrCompositionLayerBaseHeader *>(cube_layer.layer().get()));
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