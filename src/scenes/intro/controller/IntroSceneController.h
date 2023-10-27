#pragma once
#include "engine/controller/ISceneController.h"

namespace nar {
   class IntroSceneController : public ISceneController {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}