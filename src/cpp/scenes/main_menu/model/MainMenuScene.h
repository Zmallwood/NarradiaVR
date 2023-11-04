/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "core/model/IScene.h"

namespace nar {
   class MainMenuScene : public IScene {
     private:
      void UpdateGameLogic() override;
   };
}
