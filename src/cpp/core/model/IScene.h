/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
    /**
     * Base class inherited by all the game scenes.
     */
    class IScene {
      public:
        virtual void UpdateGameLogic() = 0;
    };
}
