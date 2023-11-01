/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   class InputActionsPollerController : public Singleton<InputActionsPollerController> {
     public:
      void PollInputActions();
   };
}