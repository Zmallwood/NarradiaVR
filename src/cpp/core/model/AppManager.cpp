/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "AppManager.h"

namespace nar {
    /**
     * Initialize at game start, setting properties of app object:
     */
    void AppManager::Init() {
        JNIEnv *env;
        app_->activity->vm->AttachCurrentThread(&env, nullptr);
        app_->userData = &appState_;
        app_->onAppCmd = HandleAppCmd;
    }

    /**
     * Cleanup at game exit.
     */
    void AppManager::Cleanup() {
        app_->activity->vm->DetachCurrentThread();
    }

    /**
     * Process the next main command.
     */
    void AppManager::HandleAppCmd(struct android_app *app, int32_t cmd) {
        AppState *app_state = (AppState *)app->userData;

        switch (cmd) {
        /*
         * There is no APP_CMD_CREATE. The ANativeActivity creates the
         * application thread from onCreate(). The application thread
         * then calls android_main().
         */
        case APP_CMD_START:
            break;
        case APP_CMD_RESUME:
            app_state->resumed = true;
            break;
        case APP_CMD_PAUSE:
            app_state->resumed = false;
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_DESTROY:
            app_state->native_window = NULL;
            break;
        case APP_CMD_INIT_WINDOW:
            app_state->native_window = app->window;
            break;
        case APP_CMD_TERM_WINDOW:
            app_state->native_window = NULL;
            break;
        }
    }
}
