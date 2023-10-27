#pragma once
#include "engine/view/ISceneView.h"

namespace nar {
   class IntroSceneView : public ISceneView {
     public:
      void Render() override;
   };
}