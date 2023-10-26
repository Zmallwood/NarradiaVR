#pragma once
#include "common/gfxwrapper_opengl.h"
#include "matter/struct_Cube.h"

namespace nar {
   struct Options;
   class AndroidPlatform;

   class GraphicsGL : public Singleton<GraphicsGL> {
     public:
      GraphicsGL();
      GraphicsGL(const GraphicsGL &) = delete;
      GraphicsGL &operator=(const GraphicsGL &) = delete;
      GraphicsGL(GraphicsGL &&) = delete;
      GraphicsGL &operator=(GraphicsGL &&) = delete;
      ~GraphicsGL();
      std::vector<std::string> GetInstanceExtensions() const;
      void DebugMessageCallback(
          GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
          const GLchar *message);
      void InitializeDevice(XrInstance instance, XrSystemId system_id);
      void InitializeResources();
      void CheckShader(GLuint shader);
      void CheckProgram(GLuint prog);
      int64_t SelectColorSwapchainFormat(const std::vector<int64_t> &runtime_formats) const;
      const XrBaseInStructure *GetGraphicsBinding() const;
      std::vector<XrSwapchainImageBaseHeader *> AllocateSwapchainImageStructs(
          uint32_t capacity, const XrSwapchainCreateInfo & /*swapchainCreateInfo*/);
      uint32_t GetDepthTexture(uint32_t color_texture);
      void RenderView(
          const XrCompositionLayerProjectionView &layer_view,
          const XrSwapchainImageBaseHeader *swapchain_image, int64_t swapchain_format,
          const std::vector<Cube> &cubes);
      uint32_t GetSupportedSwapchainSampleCount(const XrViewConfigurationView &);
      void UpdateOptions();

      ksGpuWindow window;

     private:
      XrGraphicsBindingOpenGLESAndroidKHR graphics_binding_ = {
          XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR};
      std::list<std::vector<XrSwapchainImageOpenGLESKHR>> swapchain_image_buffers_;
      GLuint swapchain_framebuffer_ = 0;
      GLuint program_ = 0;
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
   };
}