#pragma once

namespace nar {
   class InputActionsHandler : public Singleton<InputActionsHandler> {
     public:
      void InitInputActions();
      void PollInputActions();
   };
}