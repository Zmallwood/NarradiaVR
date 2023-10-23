#include "Pch.h"
#include "SingletonManager.h"

namespace nar {

  static std::vector<std::function<void()>> gDisposeActions;

  void AddSingletonDisposeAction(std::function<void()> action) {
    gDisposeActions.push_back(action);
    auto strNum = "Number of singletons: " + std::to_string(gDisposeActions.size());
    __android_log_print(ANDROID_LOG_ERROR, "Narradia", "%s", strNum.c_str());
  }

  void DisposeAllSingletons() {
    auto str = "Number of singletons: " + std::to_string(gDisposeActions.size());
    __android_log_print(ANDROID_LOG_ERROR, "Narradia", "%s", str.c_str());
    for (auto &dispose_action : gDisposeActions) {
      dispose_action();
    }
  }
}