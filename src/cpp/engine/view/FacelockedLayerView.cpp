/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "FacelockedLayerView.h"
#include "../model/OpenXrProgram.h"
#include "engine/model/OptionsManager.h"
#include "engine/view/GraphicsGLView.h"
#include "engine/view/SwapchainManagerView.h"
#include "matter/model/Cube.h"

namespace nar {
    bool FacelockedLayerView::Render(
        XrTime predicted_display_time,
        std::vector<XrCompositionLayerProjectionView> &projection_layer_views,
        std::function<void(XrMatrix4x4f)> gl_facelocked_render_code) {
        *layer_ = {XR_TYPE_COMPOSITION_LAYER_PROJECTION};

        XrResult res;

        XrViewState view_state = {XR_TYPE_VIEW_STATE};
        uint32_t view_capacity_input = (uint32_t)SwapchainManagerView::Get()->views().size();
        uint32_t view_count_output;

        XrViewLocateInfo view_locate_info = {XR_TYPE_VIEW_LOCATE_INFO};
        view_locate_info.viewConfigurationType =
            OptionsManager::Get()->options()->Parsed.view_config_type;
        view_locate_info.displayTime = predicted_display_time;
        view_locate_info.space = OpenXrProgram::Get()->app_space();

        std::vector<XrView> views = SwapchainManagerView::Get()->views();

        res = xrLocateViews(
            OpenXrProgram::Get()->session(), &view_locate_info, &view_state, view_capacity_input,
            &view_count_output, views.data());

        if (XR_FAILED(res)) {
            __android_log_print(
                ANDROID_LOG_ERROR, "Narradia",
                "XrResult failure: xrLocateViews in CubeLayerView::Render");
            return false;
        }

        SwapchainManagerView::Get()->set_views(views);

        if ((view_state.viewStateFlags & XR_VIEW_STATE_POSITION_VALID_BIT) == 0 ||
            (view_state.viewStateFlags & XR_VIEW_STATE_ORIENTATION_VALID_BIT) == 0) {
            return false; // There is no valid tracking poses for the views.
        }

        projection_layer_views.resize(view_count_output);

        // Render view to the appropriate part of the swapchain image.
        for (uint32_t i = 0; i < view_count_output; i++) {
            // Each view has a separate swapchain which is acquired, rendered to, and
            // released.
            const Swapchain view_swapchain = SwapchainManagerView::Get()->swapchains()[i];

            XrSwapchainImageAcquireInfo acquire_info = {XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};

            uint32_t swapchain_image_index;

            xrAcquireSwapchainImage(view_swapchain.handle, &acquire_info, &swapchain_image_index);

            XrSwapchainImageWaitInfo wait_info = {XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
            wait_info.timeout = XR_INFINITE_DURATION;
            xrWaitSwapchainImage(view_swapchain.handle, &wait_info);

            projection_layer_views[i] = {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW};
            projection_layer_views[i].pose = SwapchainManagerView::Get()->views()[i].pose;
            projection_layer_views[i].fov = SwapchainManagerView::Get()->views()[i].fov;
            projection_layer_views[i].subImage.swapchain = view_swapchain.handle;
            projection_layer_views[i].subImage.imageRect.offset = {0, 0};
            projection_layer_views[i].subImage.imageRect.extent = {
                view_swapchain.width, view_swapchain.height};

            const XrSwapchainImageBaseHeader *const swapchain_image =
                SwapchainManagerView::Get()
                    ->swapchain_images()[view_swapchain.handle][swapchain_image_index];
            Point3F player_translation = {0.0f, 0.0f, 0.0f};
            GraphicsGLView::Get()->RenderView(
                projection_layer_views[i], swapchain_image,
                SwapchainManagerView::Get()->color_swapchain_format(), gl_facelocked_render_code,
                player_translation, true);

            XrSwapchainImageReleaseInfo release_info = {XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            xrReleaseSwapchainImage(view_swapchain.handle, &release_info);
        }

        layer_->space = OpenXrProgram::Get()->app_space();
        layer_->layerFlags = OptionsManager::Get()->options()->Parsed.environment_blend_mode ==
                                     XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND
                                 ? XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT |
                                       XR_COMPOSITION_LAYER_UNPREMULTIPLIED_ALPHA_BIT
                                 : 0;
        layer_->viewCount = (uint32_t)projection_layer_views.size();
        layer_->views = projection_layer_views.data();

        return true;
    }
}
