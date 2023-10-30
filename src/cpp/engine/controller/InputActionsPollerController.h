#pragma once

namespace nar {
   class InputActionsPollerController : public Singleton<InputActionsPollerController> {
     public:
      void PollInputActions();
   };
}