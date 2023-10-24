#pragma once

namespace nar {
  //
  // Functions related to disposing of Singletons at game exit.
  //
  void AddSingletonDisposeAction(std::function<void()> action);
  void DisposeAllSingletons();
}