/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "SingletonsCleanup.h"
#include <vector>

namespace nar {

    static std::vector<std::function<void()>> gDisposeActions;

    /**
     * Called everytime a singleton is created.
     */
    auto AddSingletonDisposeAction(std::function<void()> action) -> void {

        gDisposeActions.push_back(action);
    }

    /**
     * Called at game exit.
     */
    auto DisposeAllSingletons() -> void {

        for (auto &dispose_action : gDisposeActions)
            dispose_action();
    }
}
