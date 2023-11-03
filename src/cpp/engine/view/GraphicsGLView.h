/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "matter/model/Cube.h"
#include "ExmplShaderProgView.h"

namespace nar {
   struct Options;
   class AndroidPlatform;

   class GraphicsGLView : public Singleton<GraphicsGLView> {
     public:
      GraphicsGLView();
      ~GraphicsGLView();
      std::vector<std::string> GetInstanceExtensions() const;
      void InitializeDevice(XrInstance instance, XrSystemId system_id);
      int64_t SelectColorSwapchainFormat(const std::vector<int64_t> &runtime_formats) const;
      const XrBaseInStructure *GetGraphicsBinding() const;
      std::vector<XrSwapchainImageBaseHeader *> AllocateSwapchainImageStructs(
          uint32_t capacity, const XrSwapchainCreateInfo & /*swapchainCreateInfo*/);
      void RenderView(
          const XrCompositionLayerProjectionView &layer_view,
          const XrSwapchainImageBaseHeader *swapchain_image, int64_t swapchain_format,
          std::function<void(XrMatrix4x4f)> gl_render_code, Point3F player_translation = {0.0f, 0.0f, 0.0f}, bool facelocked = false);
      uint32_t GetSupportedSwapchainSampleCount(const XrViewConfigurationView &);
      void UpdateOptions();

      GLuint vao() {
         return vao_;
      }

      ExmplShaderProgView shader_program() {
         return shader_program_;
      }

     private:
      void InitializeResources();
      uint32_t GetDepthTexture(uint32_t color_texture);

      ksGpuWindow window_;
      XrGraphicsBindingOpenGLESAndroidKHR graphics_binding_ = {
          XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR};
      std::list<std::vector<XrSwapchainImageOpenGLESKHR>> swapchain_image_buffers_;
      GLuint swapchain_framebuffer_ = 0;
      GLint model_view_projection_uniform_location_ = 0;
      GLint vertex_attrib_coords_ = 0;
      GLint vertex_attrib_color_ = 0;
      GLuint vao_ = 0;
      GLuint cube_vertex_buffer_ = 0;
      GLuint cube_index_buffer_ = 0;
      GLint context_api_major_version_ = 0;
      // Map color buffer to associated depth buffer. This map is populated on demand.
      std::map<uint32_t, uint32_t> color_to_depth_map_;
      std::array<float, 4> clear_color_;
      ExmplShaderProgView shader_program_;
   };
}
