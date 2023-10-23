#include "Pch.h"
#include "AndroidPlatformPlugin.h"
#include "engine/AndroidAppManager.h"

namespace nar {
  AndroidPlatformPlugin::AndroidPlatformPlugin() {
    instance_create_info_android_ = {XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
    instance_create_info_android_.applicationVM = AndroidAppManager::Get()->app()->activity->vm;
    instance_create_info_android_.applicationActivity =
        AndroidAppManager::Get()->app()->activity->clazz;
  }

  std::vector<std::string> AndroidPlatformPlugin::GetInstanceExtensions() const {
    return {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME};
  }

  XrBaseInStructure *AndroidPlatformPlugin::GetInstanceCreateExtension() const {
    return (XrBaseInStructure *)&instance_create_info_android_;
  }
}