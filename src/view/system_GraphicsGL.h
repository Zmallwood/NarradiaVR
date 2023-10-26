/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "model/matter/struct_Cube.h"
#include "class_ShaderProgram.h"

namespace nar {
   struct Options;
   class AndroidPlatform;

   class GraphicsGL : public Singleton<GraphicsGL> {
     public:
      GraphicsGL();
      ~GraphicsGL();
      std::vector<std::string> GetInstanceExtensions() const;
      void InitializeDevice(XrInstance instance, XrSystemId system_id);
      int64_t SelectColorSwapchainFormat(const std::vector<int64_t> &runtime_formats) const;
      const XrBaseInStructure *GetGraphicsBinding() const;
      std::vector<XrSwapchainImageBaseHeader *> AllocateSwapchainImageStructs(
          uint32_t capacity, const XrSwapchainCreateInfo & /*swapchainCreateInfo*/);
      void RenderView(
          const XrCompositionLayerProjectionView &layer_view,
          const XrSwapchainImageBaseHeader *swapchain_image, int64_t swapchain_format,
          const std::vector<Cube> &cubes);
      uint32_t GetSupportedSwapchainSampleCount(const XrViewConfigurationView &);
      void UpdateOptions();

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
      ShaderProgram shader_program_;
   };
}