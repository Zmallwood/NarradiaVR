/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "engine/view/ISceneView.h"

namespace nar {
   class MainMenuSceneView : public ISceneView {
     public:
      void Render() override;
   };
}