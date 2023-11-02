/* (c) 2023 Zmallwood
:his code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
    /**
     * Top most controller object, executes the controllers below.
     */
    class EngineController : public Singleton<EngineController> {
      public:
        void HandleInput();
        void UpdateGameFlow();

      private:
        bool QuittingGameIfRequested();
        bool ThrottlingGameIfSessionNotRunning();
    };
}
