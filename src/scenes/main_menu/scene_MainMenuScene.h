#pragma once
#include "engine/core/IScene.h"

namespace nar {
  class MainMenuScene : public IScene {
   private:
    void UpdateDerived() override;
    void RenderDerived() override;
  };
}