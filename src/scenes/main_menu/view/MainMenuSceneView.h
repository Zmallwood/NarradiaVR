#pragma once
#include "engine/view/ISceneView.h"

namespace nar {
   class MainMenuSceneView : public ISceneView {
     public:
      void Render() override;
   };
}