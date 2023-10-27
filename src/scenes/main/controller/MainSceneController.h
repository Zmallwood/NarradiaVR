#pragma once
#include "engine/controller/ISceneController.h"

namespace nar {
   class MainSceneController : public ISceneController {
     public:
      void HandleInput() override;
      void UpdateGameFlow() override;
   };
}