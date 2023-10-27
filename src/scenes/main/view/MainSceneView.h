#pragma once
#include "engine/view/ISceneView.h"

namespace nar {
   class MainSceneView : public ISceneView {
     public:
      void Render() override;
   };
}