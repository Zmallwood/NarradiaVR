#pragma once

namespace nar {
   class IScene {
     public:
      void Update();
      void Render();

     protected:
      virtual void UpdateDerived() = 0;
      virtual void RenderDerived() = 0;
   };
}