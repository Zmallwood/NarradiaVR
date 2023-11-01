/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "InputState.h"

namespace nar {
   struct Options;
   class AndroidPlatform;
   class GraphicsGLView;

   class OpenXrProgram : public Singleton<OpenXrProgram> {
     public:
      OpenXrProgram();
      ~OpenXrProgram();
      void Init();
      XrEnvironmentBlendMode GetPreferredBlendMode() const;
      void
      HandleSessionStateChangedEvent(const XrEventDataSessionStateChanged &state_changed_event);

      bool IsSessionRunning() const {
         return session_running_;
      }

      bool IsSessionFocused() const {
         return session_state_ == XR_SESSION_STATE_FOCUSED;
      }

      XrSession session() {
         return session_;
      }

      XrSpace app_space() {
         return app_space_;
      }

      std::vector<XrSpace> visualized_spaces() {
         return visualized_spaces_;
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

      std::shared_ptr<GraphicsGLView> graphics_plugin() {
         return graphics_plugin_;
      }

     private:
      void CreateInstanceInternal();
      void CreateInstance();
      void InitSystem();
      void InitDevice();
      void CreateVisualizedSpaces();
      void InitSession();
      XrReferenceSpaceCreateInfo
      GetXrReferenceSpaceCreateInfo(const std::string &referenceSpaceTypeStr);

      const std::shared_ptr<const Options> options_;
      std::shared_ptr<AndroidPlatform> platform_plugin_;
      std::shared_ptr<GraphicsGLView> graphics_plugin_;
      XrInstance instance_ = {XR_NULL_HANDLE};
      XrSession session_ = {XR_NULL_HANDLE};
      XrSpace app_space_ = {XR_NULL_HANDLE};
      XrSystemId system_id_ = {XR_NULL_SYSTEM_ID};
      std::vector<XrSpace> visualized_spaces_;
      // Application's current lifecycle state according to the runtime
      XrSessionState session_state_ = {XR_SESSION_STATE_UNKNOWN};
      bool session_running_ = {false};
      XrEventDataBuffer event_data_buffer_;
      const std::set<XrEnvironmentBlendMode> kAcceptableBlendModes;
   };
}
