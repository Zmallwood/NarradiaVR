#pragma once
#include "engine/core/IScene.h"

namespace nar {
  class IntroScene : public IScene {
   private:
    void UpdateDerived() override;
    void RenderDerived() override;
  };
}