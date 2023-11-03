/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
    /**
     * Base class to be inherited by all scene controllers.
     */
    class ISceneCtrlr {
      public:
        virtual auto HandleInput() -> void = 0;
        virtual auto UpdateGameFlow() -> void = 0;
    };
}
