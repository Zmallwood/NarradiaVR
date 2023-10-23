#pragma once

namespace nar {
  void AddSingletonDisposeAction(std::function<void()> action);
  void DisposeAllSingletons();
}