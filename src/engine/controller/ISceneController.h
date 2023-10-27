#pragma once

namespace nar {
   class ISceneController {
     public:
      virtual void HandleInput() = 0;
      virtual void UpdateGameFlow() = 0;
   };
}