#pragma once

namespace nar {
   class InputActionsController : public Singleton<InputActionsController> {
     public:
      void InitInputActions();
      void PollInputActions();
   };
}