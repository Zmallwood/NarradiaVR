#pragma once

namespace nar {
  struct Options;

  class AndroidPlatformPlugin : public Singleton<AndroidPlatformPlugin> {
   public:
    AndroidPlatformPlugin();
    std::vector<std::string> GetInstanceExtensions() const;
    XrBaseInStructure *GetInstanceCreateExtension() const;

    XrInstanceCreateInfoAndroidKHR instance_create_info_android_;
  };
}