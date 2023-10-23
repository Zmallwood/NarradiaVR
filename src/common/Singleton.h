#pragma once
#include "SingletonManager.h"

namespace nar {

  ///
  /// Used for all classes that only need to be called once.
  ///
  template <class T> class Singleton {
   public:
    static void Create() {
      if (!instance_) { // Create new instance_ if none exists
        std::string str = "Creating " + std::string(typeid(T).name());
        __android_log_print(ANDROID_LOG_ERROR, "Narradia", str.c_str());
        instance_ = std::make_shared<T>();
        AddSingletonDisposeAction([&] { instance_.reset(); });
      }
    }

    static std::shared_ptr<T> Get() {
      if (!instance_) // Create new instance_ if none exists
        Create();
      return std::weak_ptr<T>(instance_).lock(); // Return weak_ptr to not increase ref count
    }

    static void Dispose() {
      std::string str = "Disposing " + std::string(typeid(T).name());
      __android_log_print(ANDROID_LOG_ERROR, "Narradia", str.c_str());
      instance_.reset(); // Delete instance_ if want to force it
    }

   private:
    inline static std::shared_ptr<T> instance_;
  };
}