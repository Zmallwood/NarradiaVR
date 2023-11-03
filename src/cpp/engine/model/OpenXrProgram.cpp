/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "OpenXrProgram.h"
#include "AndroidPlatform.h"
#include "Engine.h"
#include "HandDeviceInput.h"
#include "Options.h"
#include "OptionsManager.h"
#include "../../engine.input_actions/controller/InputActionsCreateController.h"
#include "../controller/BindingsCreateController.h"
#include "engine/view/GraphicsGLView.h"
#include "engine/view/SwapchainManagerView.h"

namespace nar {
    OpenXrProgram::OpenXrProgram()
        : options_(OptionsManager::Get()->options()),
          platform_plugin_(AndroidPlatform::Get()),
          graphics_plugin_(GraphicsGLView::Get()),
          kAcceptableBlendModes{
              XR_ENVIRONMENT_BLEND_MODE_OPAQUE, XR_ENVIRONMENT_BLEND_MODE_ADDITIVE,
              XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND} {
    }

    OpenXrProgram::~OpenXrProgram() {
        for (XrSpace visualized_space : visualized_spaces_)
            xrDestroySpace(visualized_space);

        if (app_space_ != XR_NULL_HANDLE)
            xrDestroySpace(app_space_);

        if (session_ != XR_NULL_HANDLE)
            xrDestroySession(session_);

        if (instance_ != XR_NULL_HANDLE)
            xrDestroyInstance(instance_);
    }

    void OpenXrProgram::Init() {
        CreateInstance();
        InitSystem();
        InitDevice();
        InitSession();
        SwapchainManagerView::Get()->CreateSwapchains();
    }

    void OpenXrProgram::HandleSessionStateChangedEvent(
        const XrEventDataSessionStateChanged &state_changed_event) {
        const XrSessionState old_state = session_state_;
        session_state_ = state_changed_event.state;

        if ((state_changed_event.session != XR_NULL_HANDLE) &&
            (state_changed_event.session != session_)) {
            __android_log_print(
                ANDROID_LOG_ERROR, "Narradia",
                "XrEventDataSessionStateChanged for unknown session");
            return;
        }

        switch (session_state_) {
        case XR_SESSION_STATE_READY: {
            XrSessionBeginInfo session_begin_info = {XR_TYPE_SESSION_BEGIN_INFO};
            session_begin_info.primaryViewConfigurationType = options_->Parsed.view_config_type;
            xrBeginSession(session_, &session_begin_info);
            session_running_ = true;
            break;
        }
        case XR_SESSION_STATE_STOPPING: {
            session_running_ = false;
            xrEndSession(session_);
            break;
        }
        case XR_SESSION_STATE_EXITING: {
            Engine::Get()->set_exit_render_loop(true);
            break;
        }
        case XR_SESSION_STATE_LOSS_PENDING: {
            Engine::Get()->set_exit_render_loop(true);
            break;
        }
        default:
            break;
        }
    }

    void OpenXrProgram::CreateInstanceInternal() {
        // Create union of extensions required by platform and graphics plugins.
        std::vector<const char *> extensions;

        // Transform platform and graphics extension std::strings to C strings.
        const std::vector<std::string> platform_extensions =
            platform_plugin_->GetInstanceExtensions();
        std::transform(
            platform_extensions.begin(), platform_extensions.end(), std::back_inserter(extensions),
            [](const std::string &ext) { return ext.c_str(); });
        const std::vector<std::string> graphics_extensions =
            graphics_plugin_->GetInstanceExtensions();
        std::transform(
            graphics_extensions.begin(), graphics_extensions.end(), std::back_inserter(extensions),
            [](const std::string &ext) { return ext.c_str(); });

        XrInstanceCreateInfo create_info = {XR_TYPE_INSTANCE_CREATE_INFO};
        create_info.next = platform_plugin_->GetInstanceCreateExtension();
        create_info.enabledExtensionCount = (uint32_t)extensions.size();
        create_info.enabledExtensionNames = extensions.data();

        strcpy(create_info.applicationInfo.applicationName, "HelloXR");
        create_info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

        xrCreateInstance(&create_info, &instance_);
    }

    void OpenXrProgram::CreateInstance() {
        CreateInstanceInternal();
    }

    XrEnvironmentBlendMode OpenXrProgram::GetPreferredBlendMode() const {
        uint32_t count;
        xrEnumerateEnvironmentBlendModes(
            instance_, system_id_, options_->Parsed.view_config_type, 0, &count, nullptr);

        std::vector<XrEnvironmentBlendMode> blend_modes(count);
        xrEnumerateEnvironmentBlendModes(
            instance_, system_id_, options_->Parsed.view_config_type, count, &count,
            blend_modes.data());

        for (const auto &blend_mode : blend_modes) {
            if (kAcceptableBlendModes.count(blend_mode))
                return blend_mode;
        }

        __android_log_print(
            ANDROID_LOG_ERROR, "Narradia",
            "No acceptable blend mode returned from the xrEnumerateEnvironmentBlendModes.");
    }

    void OpenXrProgram::InitSystem() {
        XrSystemGetInfo system_info = {XR_TYPE_SYSTEM_GET_INFO};
        system_info.formFactor = options_->Parsed.form_factor;
        xrGetSystem(instance_, &system_info, &system_id_);
    }

    void OpenXrProgram::InitDevice() {
        // The graphics API can initialize the graphics device now that the systemId and
        // instance handle are available.
        graphics_plugin_->InitializeDevice(instance_, system_id_);
    }

    void OpenXrProgram::CreateVisualizedSpaces() {
        std::string visualized_spaces[] = {"ViewFront",        "Local",      "Stage",
                                           "StageLeft",        "StageRight", "StageLeftRotated",
                                           "StageRightRotated"};

        for (const auto &visualized_space : visualized_spaces) {
            XrReferenceSpaceCreateInfo reference_space_create_info =
                GetXrReferenceSpaceCreateInfo(visualized_space);
            XrSpace space;
            XrResult res = xrCreateReferenceSpace(session_, &reference_space_create_info, &space);
            if (XR_SUCCEEDED(res)) {
                visualized_spaces_.push_back(space);
            }
            else {
                __android_log_print(
                    ANDROID_LOG_WARN, "Narradia",
                    "Failed to create reference space for a tile with error %d", res);
            }
        }
    }

    void OpenXrProgram::InitSession() {
        {
            XrSessionCreateInfo create_info = {XR_TYPE_SESSION_CREATE_INFO};
            create_info.next = graphics_plugin_->GetGraphicsBinding();
            create_info.systemId = system_id_;
            xrCreateSession(instance_, &create_info, &session_);
        }

        InputActionsCreateController::Get()->CreateInputActions();
        BindingsCreateController::Get()->CreateBindings();
        //      InputActionsController::Get()->InitInputActions();
        CreateVisualizedSpaces();

        {
            XrReferenceSpaceCreateInfo reference_space_create_info =
                GetXrReferenceSpaceCreateInfo(options_->app_space);
            xrCreateReferenceSpace(session_, &reference_space_create_info, &app_space_);
        }
    }

    XrReferenceSpaceCreateInfo
    OpenXrProgram::GetXrReferenceSpaceCreateInfo(const std::string &referenceSpaceTypeStr) {
        XrReferenceSpaceCreateInfo reference_space_create_info = {
            XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
        reference_space_create_info.poseInReferenceSpace = UtilView::Identity();

        if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "View")) {
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "ViewFront")) {
            // Render head-locked 2m in front of device.
            reference_space_create_info.poseInReferenceSpace =
                UtilView::Translation({0.f, 0.f, -2.f}),
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "Local")) {
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "Stage")) {
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "StageLeft")) {
            reference_space_create_info.poseInReferenceSpace =
                UtilView::RotateCCWAboutYAxis(0.f, {-2.f, 0.f, -2.f});
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "StageRight")) {
            reference_space_create_info.poseInReferenceSpace =
                UtilView::RotateCCWAboutYAxis(0.f, {2.f, 0.f, -2.f});
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "StageLeftRotated")) {
            reference_space_create_info.poseInReferenceSpace =
                UtilView::RotateCCWAboutYAxis(3.14f / 3.f, {-2.f, 0.5f, -2.f});
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        }
        else if (Util::EqualsIgnoreCase(referenceSpaceTypeStr, "StageRightRotated")) {
            reference_space_create_info.poseInReferenceSpace =
                UtilView::RotateCCWAboutYAxis(-3.14f / 3.f, {2.f, 0.5f, -2.f});
            reference_space_create_info.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
        }
        else {
            throw std::invalid_argument("Unknown reference space type: " + referenceSpaceTypeStr);
        }

        return reference_space_create_info;
    }
}
