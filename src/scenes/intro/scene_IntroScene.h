#pragma once
#include "engine/core/class_IScene.h"

namespace nar {
   class IntroScene : public IScene {
     private:
      void UpdateDerived() override;
      void RenderDerived() override;
   };
}