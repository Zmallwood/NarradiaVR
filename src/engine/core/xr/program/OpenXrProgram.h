#pragma once
#include "InputState.h"
#include "Swapchain.h"

namespace nar {
  struct Options;
  class AndroidPlatformPlugin;
  class OpenGLESGraphicsPlugin;

  class OpenXrProgram : public Singleton<OpenXrProgram> {
   public:
    OpenXrProgram();
    ~OpenXrProgram();
    void Initialize();
    XrEnvironmentBlendMode GetPreferredBlendMode() const;
    void PollInputActions();
    void HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged &state_changed_event);

    bool IsSessionRunning() const {
      return session_running_;
    }

    bool IsSessionFocused() const {
      return session_state_ == XR_SESSION_STATE_FOCUSED;
    }

    XrSession session() {
      return session_;
    }

    std::vector<XrView> views() {
      return views_;
    }

    void set_views(std::vector<XrView> value) {
      views_ = value;
    }

    XrSpace app_space() {
      return app_space_;
    }

    std::vector<XrViewConfigurationView> config_views() {
      return config_views_;
    }

    std::vector<Swapchain> swapchains() {
      return swapchains_;
    }

    std::vector<XrSpace> visualized_spaces() {
      return visualized_spaces_;
    }

    InputState input() {
      return input_;
    }

    std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader *>> swapchain_images() {
      return swapchain_images_;
    }

    int64_t color_swapchain_format() {
      return color_swapchain_format_;
    }

    XrInstance instance() {
      return instance_;
    }

    XrSystemId system_id() {
      return system_id_;
    }

    XrEventDataBuffer event_data_buffer() {
      return event_data_buffer_;
    }

    XrSessionState session_state() {
      return session_state_;
    }

   private:
    void CreateInstanceInternal();
    void CreateInstance();
    void InitializeSystem();
    void InitializeDevice();
    void InitializeActions();
    void CreateVisualizedSpaces();
    void InitializeSession();
    void CreateSwapchains();

    const std::shared_ptr<const Options> options_;
    std::shared_ptr<AndroidPlatformPlugin> platform_plugin_;
    std::shared_ptr<OpenGLESGraphicsPlugin> graphics_plugin_;
    XrInstance instance_ = {XR_NULL_HANDLE};
    XrSession session_ = {XR_NULL_HANDLE};
    XrSpace app_space_ = {XR_NULL_HANDLE};
    XrSystemId system_id_ = {XR_NULL_SYSTEM_ID};
    std::vector<XrViewConfigurationView> config_views_;
    std::vector<Swapchain> swapchains_;
    std::map<XrSwapchain, std::vector<XrSwapchainImageBaseHeader *>> swapchain_images_;
    std::vector<XrView> views_;
    int64_t color_swapchain_format_ = {-1};
    std::vector<XrSpace> visualized_spaces_;
    // Application's current lifecycle state according to the runtime
    XrSessionState session_state_ = {XR_SESSION_STATE_UNKNOWN};
    bool session_running_ = {false};
    XrEventDataBuffer event_data_buffer_;
    InputState input_;
    const std::set<XrEnvironmentBlendMode> kAcceptableBlendModes;
  };
}