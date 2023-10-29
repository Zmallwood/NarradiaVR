/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "GraphicsGLView.h"
#include "engine/model/ModelBank.h"
#include "engine/model/Options.h"
#include "engine/model/OptionsManager.h"
#include "world/model/Player.h"

namespace nar {
   GraphicsGLView::GraphicsGLView()
       : clear_color_(OptionsManager::Get()->options()->GetBackgroundClearColor()) {
   }

   GraphicsGLView::~GraphicsGLView() {
      if (swapchain_framebuffer_ != 0)
         glDeleteFramebuffers(1, &swapchain_framebuffer_);

      shader_program_.Cleanup();

      if (vao_ != 0)
         glDeleteVertexArrays(1, &vao_);

      if (cube_vertex_buffer_ != 0)
         glDeleteBuffers(1, &cube_vertex_buffer_);

      if (cube_index_buffer_ != 0)
         glDeleteBuffers(1, &cube_index_buffer_);

      for (auto &colorToDepth : color_to_depth_map_) {
         if (colorToDepth.second != 0)
            glDeleteTextures(1, &colorToDepth.second);
      }

      ksGpuWindow_Destroy(&window_);
   }

   std::vector<std::string> GraphicsGLView::GetInstanceExtensions() const {
      return {XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME};
   }

   void GraphicsGLView::InitializeDevice(XrInstance instance, XrSystemId system_id) {
      __android_log_print(ANDROID_LOG_ERROR, "Narradia", "init graphics");
      // Extension function must be loaded by name
      PFN_xrGetOpenGLESGraphicsRequirementsKHR pfn_get_open_gles_graphics_requirements_khr =
          nullptr;
      xrGetInstanceProcAddr(
          instance, "xrGetOpenGLESGraphicsRequirementsKHR",
          reinterpret_cast<PFN_xrVoidFunction *>(&pfn_get_open_gles_graphics_requirements_khr));

      XrGraphicsRequirementsOpenGLESKHR graphics_requirements = {
          XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR};
      pfn_get_open_gles_graphics_requirements_khr(instance, system_id, &graphics_requirements);

      // Initialize the gl extensions. Note we have to open a window.
      ksDriverInstance driver_instance = {};
      ksGpuQueueInfo queue_info = {};
      ksGpuSurfaceColorFormat color_format = {KS_GPU_SURFACE_COLOR_FORMAT_B8G8R8A8};
      ksGpuSurfaceDepthFormat depth_format = {KS_GPU_SURFACE_DEPTH_FORMAT_D24};
      ksGpuSampleCount sample_count = {KS_GPU_SAMPLE_COUNT_1};

      if (!ksGpuWindow_Create(
              &window_, &driver_instance, &queue_info, 0, color_format, depth_format, sample_count,
              640, 480, false)) {
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Unable to create GL context.");
         return;
      }

      GLint major = 0;
      GLint minor = 0;
      glGetIntegerv(GL_MAJOR_VERSION, &major);
      glGetIntegerv(GL_MINOR_VERSION, &minor);

      const XrVersion desired_api_version = XR_MAKE_VERSION(major, minor, 0);

      if (graphics_requirements.minApiVersionSupported > desired_api_version) {
         __android_log_print(
             ANDROID_LOG_ERROR, "Narradia",
             "Runtime does not support desired Graphics API and/or version.");
         return;
      }

      context_api_major_version_ = major;

      graphics_binding_.display = window_.display;
      graphics_binding_.config = (EGLConfig)0;
      graphics_binding_.context = window_.context.context;

      glEnable(GL_DEBUG_OUTPUT);

      InitializeResources();
   }

   void GraphicsGLView::InitializeResources() {
      shader_program_.Init();

      auto vertex_cube = ModelBank::Get()->vertex_cube();

      glGenFramebuffers(1, &swapchain_framebuffer_);

      model_view_projection_uniform_location_ =
          glGetUniformLocation(shader_program_.program(), "ModelViewProjection");

      vertex_attrib_coords_ = glGetAttribLocation(shader_program_.program(), "VertexPos");
      vertex_attrib_color_ = glGetAttribLocation(shader_program_.program(), "VertexColor");

      glGenBuffers(1, &cube_vertex_buffer_);
      glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer_);
      glBufferData(
          GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_cube.vertices().size(),
          vertex_cube.vertices().data(), GL_STATIC_DRAW);

      glGenBuffers(1, &cube_index_buffer_);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_buffer_);
      glBufferData(
          GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * vertex_cube.indices().size(),
          vertex_cube.indices().data(), GL_STATIC_DRAW);

      glGenVertexArrays(1, &vao_);
      glBindVertexArray(vao_);
      glEnableVertexAttribArray(vertex_attrib_coords_);
      glEnableVertexAttribArray(vertex_attrib_color_);
      glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_buffer_);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_index_buffer_);
      glVertexAttribPointer(vertex_attrib_coords_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
      glVertexAttribPointer(
          vertex_attrib_color_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
          reinterpret_cast<const void *>(sizeof(XrVector3f)));
   }

   int64_t
   GraphicsGLView::SelectColorSwapchainFormat(const std::vector<int64_t> &runtime_formats) const {
      // List of supported color swapchain formats.
      std::vector<int64_t> supported_color_swapchain_formats = {GL_RGBA8, GL_RGBA8_SNORM};

      // In OpenGLES 3.0+, the R, G, and B values after blending are converted into the
      // non-linear sRGB automatically.
      if (context_api_major_version_ >= 3)
         supported_color_swapchain_formats.push_back(GL_SRGB8_ALPHA8);

      auto swapchain_format_it = std::find_first_of(
          runtime_formats.begin(), runtime_formats.end(), supported_color_swapchain_formats.begin(),
          supported_color_swapchain_formats.end());

      if (swapchain_format_it == runtime_formats.end()) {
         __android_log_print(
             ANDROID_LOG_ERROR, "Narradia",
             "No runtime swapchain format supported for color swapchain");
      }

      return *swapchain_format_it;
   }

   const XrBaseInStructure *GraphicsGLView::GetGraphicsBinding() const {
      return reinterpret_cast<const XrBaseInStructure *>(&graphics_binding_);
   }

   std::vector<XrSwapchainImageBaseHeader *> GraphicsGLView::AllocateSwapchainImageStructs(
       uint32_t capacity, const XrSwapchainCreateInfo & /*swapchainCreateInfo*/) {
      // Allocate and initialize the buffer of image structs (must be sequential in memory
      // for xrEnumerateSwapchainImages). Return back an array of pointers to each
      // swapchain image struct so the consumer doesn't need to know the type/size.
      std::vector<XrSwapchainImageOpenGLESKHR> swapchain_image_buffer(
          capacity, {XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR});
      std::vector<XrSwapchainImageBaseHeader *> swapchain_image_base;

      for (XrSwapchainImageOpenGLESKHR &image : swapchain_image_buffer)
         swapchain_image_base.push_back(reinterpret_cast<XrSwapchainImageBaseHeader *>(&image));

      // Keep the buffer alive by moving it into the list of buffers.
      swapchain_image_buffers_.push_back(std::move(swapchain_image_buffer));

      return swapchain_image_base;
   }

   uint32_t GraphicsGLView::GetDepthTexture(uint32_t color_texture) {
      // If a depth-stencil view has already been created for this back-buffer, use it.
      auto depth_buffer_it = color_to_depth_map_.find(color_texture);

      if (depth_buffer_it != color_to_depth_map_.end())
         return depth_buffer_it->second;

      // This back-buffer has no corresponding depth-stencil texture, so create one with
      // matching dimensions.

      GLint width;
      GLint height;
      glBindTexture(GL_TEXTURE_2D, color_texture);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

      uint32_t depth_texture;
      glGenTextures(1, &depth_texture);
      glBindTexture(GL_TEXTURE_2D, depth_texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(
          GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT,
          GL_UNSIGNED_INT, nullptr);

      color_to_depth_map_.insert(std::make_pair(color_texture, depth_texture));

      return depth_texture;
   }

   void GraphicsGLView::RenderView(
       const XrCompositionLayerProjectionView &layer_view,
       const XrSwapchainImageBaseHeader *swapchain_image, int64_t swapchain_format,
       std::function<void(XrMatrix4x4f)> gl_code) {
      auto vertex_cube = ModelBank::Get()->vertex_cube();

      if (layer_view.subImage.imageArrayIndex != 0) {
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Texture arrays not supported.");
         return;
      }

      UNUSED_PARM(swapchain_format); // Not used in this function for now.

      glBindFramebuffer(GL_FRAMEBUFFER, swapchain_framebuffer_);

      const uint32_t color_texture =
          reinterpret_cast<const XrSwapchainImageOpenGLESKHR *>(swapchain_image)->image;

      glViewport(
          static_cast<GLint>(layer_view.subImage.imageRect.offset.x),
          static_cast<GLint>(layer_view.subImage.imageRect.offset.y),
          static_cast<GLsizei>(layer_view.subImage.imageRect.extent.width),
          static_cast<GLsizei>(layer_view.subImage.imageRect.extent.height));

      glFrontFace(GL_CW);
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);

      const uint32_t depth_texture = GetDepthTexture(color_texture);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

      // Clear swapchain and depth buffer.
      glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
      glClearDepthf(1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      // Set shaders and uniform variables.
      glUseProgram(shader_program_.program());

      const auto &pose = layer_view.pose;
      XrMatrix4x4f proj;
      XrMatrix4x4f_CreateProjectionFov(&proj, GRAPHICS_OPENGL_ES, layer_view.fov, 0.05f, 100.0f);
      XrMatrix4x4f to_view;
      XrVector3f scale{1.f, 1.f, 1.f};
      XrMatrix4x4f rotation_mat;
      XrMatrix4x4f_CreateRotation(&rotation_mat, 0.0f, Player::Get()->facing_angle_degrees, 0.0f);
      XrQuaternionf rotation;
      XrMatrix4x4f_GetRotation(&rotation, &rotation_mat);
      XrQuaternionf result;
      XrQuaternionf_Multiply(&result, &pose.orientation, &rotation);
      XrMatrix4x4f_CreateTranslationRotationScale(&to_view, &pose.position, &result, &scale);
      XrMatrix4x4f view;
      XrMatrix4x4f_InvertRigidBody(&view, &to_view);
      XrMatrix4x4f vp;
      XrMatrix4x4f_Multiply(&vp, &proj, &view);

      gl_code(vp);

      // // Set cube primitive data.
      // glBindVertexArray(vao_);

      // // Render each cube
      // for (const Cube &cube : cubes) {
      //    // Compute the model-view-projection transform and set it..
      //    XrMatrix4x4f model;
      //    XrMatrix4x4f_CreateTranslationRotationScale(
      //        &model, &cube.pose.position, &cube.pose.orientation, &cube.scale);
      //    XrMatrix4x4f mvp;
      //    XrMatrix4x4f_Multiply(&mvp, &vp, &model);
      //    glUniformMatrix4fv(
      //        model_view_projection_uniform_location_, 1, GL_FALSE,
      //        reinterpret_cast<const GLfloat *>(&mvp));

      //    // Draw the cube.
      //    glDrawElements(
      //        GL_TRIANGLES, static_cast<GLsizei>(vertex_cube.indices().size()), GL_UNSIGNED_SHORT,
      //        nullptr);
      // }

      // glBindVertexArray(0);
      // glUseProgram(0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   uint32_t GraphicsGLView::GetSupportedSwapchainSampleCount(const XrViewConfigurationView &) {
      return 1;
   }

   void GraphicsGLView::UpdateOptions() {
      clear_color_ = OptionsManager::Get()->options()->GetBackgroundClearColor();
   }
}