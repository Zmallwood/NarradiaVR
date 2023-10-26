#include "Pch.h"
#include "class_Singleton.h"

namespace nar {
   static std::vector<std::function<void()>> gDisposeActions;

   /**
    * Called everytime a singleton is created.
    */
   void AddSingletonDisposeAction(std::function<void()> action) {
      gDisposeActions.push_back(action);
   }

   /**
    * Called at game exit.
    */
   void DisposeAllSingletons() {
      for (auto &dispose_action : gDisposeActions)
         dispose_action();
   }
}