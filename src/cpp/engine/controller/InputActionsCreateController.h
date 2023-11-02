#pragma once

namespace nar {
    /**
     * Creates input actions, primarily to be able to read hand device input:
     */
    class InputActionsCreateController : public Singleton<InputActionsCreateController> {
      public:
        void CreateInputActions();

      private:
        void SetCharArray64(char (&array)[64], std::string text);
        void SetCharArray128(char (&array)[128], std::string text);
    };
}
