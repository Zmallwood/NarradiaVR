#pragma once

namespace nar {
   struct Swapchain {
      XrSwapchain handle;
      int32_t width;
      int32_t height;
   };
}