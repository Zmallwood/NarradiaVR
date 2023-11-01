/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../../engine/controller/ISceneController.h"

namespace nar {
   class MainMenuSceneController : public ISceneController {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}
