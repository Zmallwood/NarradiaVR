/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../../core/controller/ISceneCtrlr.h"

namespace nar {
   class MainMenuSceneController : public ISceneCtrlr {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}
