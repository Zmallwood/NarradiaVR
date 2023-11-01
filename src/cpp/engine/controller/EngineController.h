/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
    class EngineController : public Singleton<EngineController> {
      public:
        auto HandleInput() -> void;
        auto UpdateGameFlow() -> void;

      private:
        auto QuittingGameIfRequested() -> bool;
        auto ThrottlingGameIfSessionNotRunning() -> bool;
    };
}
