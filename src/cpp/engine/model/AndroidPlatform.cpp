/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "AndroidPlatform.h"
#include "AndroidVRAppManager.h"

namespace nar {
   AndroidPlatform::AndroidPlatform() {
      instance_create_info_android_ = {XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
      instance_create_info_android_.applicationVM = AndroidVRAppManager::Get()->app()->activity->vm;
      instance_create_info_android_.applicationActivity =
          AndroidVRAppManager::Get()->app()->activity->clazz;
   }

   std::vector<std::string> AndroidPlatform::GetInstanceExtensions() const {
      return {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME};
   }

   XrBaseInStructure *AndroidPlatform::GetInstanceCreateExtension() const {
      return (XrBaseInStructure *)&instance_create_info_android_;
   }
}
