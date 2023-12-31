/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "Loader.h"
#include "AppManager.h"

namespace nar {
   void Loader::Init() {
      PFN_xrInitializeLoaderKHR initialize_loader = // Initialize the loader for this platform
          nullptr;

      auto loader_result = xrGetInstanceProcAddr(
          XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction *)(&initialize_loader));

      if (XR_SUCCEEDED(loader_result)) {
         XrLoaderInitInfoAndroidKHR loader_init_info_android = {
             XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};

         loader_init_info_android.applicationVM = AppManager::Get()->app()->activity->vm;
         loader_init_info_android.applicationContext =
             AppManager::Get()->app()->activity->clazz;

         initialize_loader((const XrLoaderInitInfoBaseHeaderKHR *)&loader_init_info_android);
      }
   }
}
