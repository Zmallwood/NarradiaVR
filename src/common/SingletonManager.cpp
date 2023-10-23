#include "Pch.h"
#include "SingletonManager.h"

namespace nar {

  static std::vector<std::function<void()>> gDisposeActions;

  void AddSingletonDisposeAction(std::function<void()> action) {
    gDisposeActions.push_back(action);
  }

  void DisposeAllSingletons() {
    for (auto &dispose_action : gDisposeActions)
      dispose_action();
  }
}