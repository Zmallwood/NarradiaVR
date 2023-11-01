/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "SwapchainManagerView.h"
#include "engine/model/OpenXrProgram.h"
#include "engine/model/OptionsManager.h"
#include "engine/view/GraphicsGLView.h"

namespace nar {

   SwapchainManagerView::~SwapchainManagerView() {
      for (Swapchain swapchain : swapchains_)
         xrDestroySwapchain(swapchain.handle);
   }

   void SwapchainManagerView::CreateSwapchains() {
      auto instance = OpenXrProgram::Get()->instance();
      auto system_id = OpenXrProgram::Get()->system_id();
      auto session = OpenXrProgram::Get()->session();
      auto graphics_plugin = OpenXrProgram::Get()->graphics_plugin();

      // Read graphics properties for preferred swapchain length and logging.
      XrSystemProperties system_properties = {XR_TYPE_SYSTEM_PROPERTIES};
      xrGetSystemProperties(instance, system_id, &system_properties);

      // Note: No other view configurations exist at the time this code was written. If
      // this condition is not met, the project will need to be audited to see how support
      // should be added.
      if (OptionsManager::Get()->options()->Parsed.view_config_type !=
          XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO) {
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Unsupported view configuration type.");
         return;
      }

      // Query and cache view configuration views.
      uint32_t view_count;
      xrEnumerateViewConfigurationViews(
          instance, system_id, OptionsManager::Get()->options()->Parsed.view_config_type, 0,
          &view_count, nullptr);
      config_views_.resize(view_count, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
      xrEnumerateViewConfigurationViews(
          instance, system_id, OptionsManager::Get()->options()->Parsed.view_config_type,
          view_count, &view_count, config_views_.data());

      // Create and cache view buffer for xrLocateViews later.
      views_.resize(view_count, {XR_TYPE_VIEW});

      // Create the swapchain and get the images.
      if (view_count > 0) {
         // Select a swapchain format.
         uint32_t swapchain_format_count;
         xrEnumerateSwapchainFormats(session, 0, &swapchain_format_count, nullptr);
         std::vector<int64_t> swapchain_formats(swapchain_format_count);
         xrEnumerateSwapchainFormats(
             session, (uint32_t)swapchain_formats.size(), &swapchain_format_count,
             swapchain_formats.data());
         color_swapchain_format_ = graphics_plugin->SelectColorSwapchainFormat(swapchain_formats);

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
                graphics_plugin->GetSupportedSwapchainSampleCount(vp);
            swapchain_create_info.usageFlags =
                XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
            Swapchain swapchain;
            swapchain.width = swapchain_create_info.width;
            swapchain.height = swapchain_create_info.height;
            xrCreateSwapchain(session, &swapchain_create_info, &swapchain.handle);

            swapchains_.push_back(swapchain);

            uint32_t image_count;
            xrEnumerateSwapchainImages(swapchain.handle, 0, &image_count, nullptr);
            // XXX This should really just return XrSwapchainImageBaseHeader*
            std::vector<XrSwapchainImageBaseHeader *> swapchain_images =
                graphics_plugin->AllocateSwapchainImageStructs(image_count, swapchain_create_info);
            xrEnumerateSwapchainImages(
                swapchain.handle, image_count, &image_count, swapchain_images[0]);

            swapchain_images_.insert(std::make_pair(swapchain.handle, std::move(swapchain_images)));
         }
      }
   }
}
