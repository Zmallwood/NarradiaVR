/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "core/model/Swapchain.h"

namespace nar {
   class SwapchainManagerView : public Ston<SwapchainManagerView> {
     public:
      ~SwapchainManagerView();
      void CreateSwapchains();

      std::vector<Swapchain> swapchains() {
         return swapchains_;
      }

      std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader *>> swapchain_images() {
         return swapchain_images_;
      }

      int64_t color_swapchain_format() {
         return color_swapchain_format_;
      }

      std::vector<XrView> views() {
         return views_;
      }

      void set_views(std::vector<XrView> value) {
         views_ = value;
      }

     private:
      std::vector<Swapchain> swapchains_;
      std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader *>> swapchain_images_;
      int64_t color_swapchain_format_ = {-1};
      std::vector<XrViewConfigurationView> config_views_;
      std::vector<XrView> views_;
   };
}
