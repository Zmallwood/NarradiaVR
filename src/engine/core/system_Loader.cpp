#include "Pch.h"
#include "system_Loader.h"
#include "engine/system_AndroidAppManager.h"

namespace nar {
   void Loader::Init() {
      PFN_xrInitializeLoaderKHR initialize_loader = // Initialize the loader for this platform
          nullptr;

      auto loader_result = xrGetInstanceProcAddr(
          XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction *)(&initialize_loader));

      if (XR_SUCCEEDED(loader_result)) {
         XrLoaderInitInfoAndroidKHR loader_init_info_android = {
             XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};

         loader_init_info_android.applicationVM = AndroidAppManager::Get()->app()->activity->vm;
         loader_init_info_android.applicationContext =
             AndroidAppManager::Get()->app()->activity->clazz;

         initialize_loader((const XrLoaderInitInfoBaseHeaderKHR *)&loader_init_info_android);
      }
   }
}