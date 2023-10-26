/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "system_AndroidPlatform.h"
#include "engine/system_AndroidAppManager.h"

namespace nar {
   AndroidPlatform::AndroidPlatform() {
      instance_create_info_android_ = {XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
      instance_create_info_android_.applicationVM = AndroidAppManager::Get()->app()->activity->vm;
      instance_create_info_android_.applicationActivity =
          AndroidAppManager::Get()->app()->activity->clazz;
   }

   std::vector<std::string> AndroidPlatform::GetInstanceExtensions() const {
      return {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME};
   }

   XrBaseInStructure *AndroidPlatform::GetInstanceCreateExtension() const {
      return (XrBaseInStructure *)&instance_create_info_android_;
   }
}