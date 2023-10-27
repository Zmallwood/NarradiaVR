#pragma once
#include "engine/controller/ISceneController.h"

namespace nar {
   class MainMenuSceneController : public ISceneController {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}