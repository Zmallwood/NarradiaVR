#pragma once

namespace nar {
   struct Options;

   class AndroidPlatform : public Singleton<AndroidPlatform> {
     public:
      AndroidPlatform();
      std::vector<std::string> GetInstanceExtensions() const;
      XrBaseInStructure *GetInstanceCreateExtension() const;

      XrInstanceCreateInfoAndroidKHR instance_create_info_android_;
   };
}