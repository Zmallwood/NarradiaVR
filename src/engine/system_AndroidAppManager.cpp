/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_AndroidAppManager.h"
#include "system_VRApp.h"

namespace nar {
   void AndroidAppManager::Init() {
      JNIEnv *env;
      app_->activity->vm->AttachCurrentThread(&env, nullptr);
      app_->userData = &appState_;
      app_->onAppCmd = VRApp::HandleAppCmd;
   }

   void AndroidAppManager::Cleanup() {
      app_->activity->vm->DetachCurrentThread();
   }
}