/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../../engine/controller/ISceneCtrlr.h"

namespace nar {
   class MainSceneController : public ISceneCtrlr {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}
