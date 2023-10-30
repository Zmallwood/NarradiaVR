/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   void AddSingletonDisposeAction(std::function<void()> action);
   void DisposeAllSingletons();
}