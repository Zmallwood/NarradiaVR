/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "Loader.h"
#include "AndroidVRAppManager.h"

namespace nar {
   void Loader::Init() {
      PFN_xrInitializeLoaderKHR initialize_loader = // Initialize the loader for this platform
          nullptr;

      auto loader_result = xrGetInstanceProcAddr(
          XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction *)(&initialize_loader));

      if (XR_SUCCEEDED(loader_result)) {
         XrLoaderInitInfoAndroidKHR loader_init_info_android = {
             XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};

         loader_init_info_android.applicationVM = AndroidVRAppManager::Get()->app()->activity->vm;
         loader_init_info_android.applicationContext =
             AndroidVRAppManager::Get()->app()->activity->clazz;

         initialize_loader((const XrLoaderInitInfoBaseHeaderKHR *)&loader_init_info_android);
         //  auto a = loader_init_info_android.androidAssetMnager;
      }
   }
}