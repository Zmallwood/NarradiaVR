/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "SingletonsCleanup.h"

namespace nar {
   static std::vector<std::function<void()>> gDisposeActions;

   /// Called everytime a singleton is created.
   void AddSingletonDisposeAction(std::function<void()> action) {
      gDisposeActions.push_back(action);
   }
   /// Called at game exit.
   void DisposeAllSingletons() {
      for (auto &dispose_action : gDisposeActions)
         dispose_action();
   }
}