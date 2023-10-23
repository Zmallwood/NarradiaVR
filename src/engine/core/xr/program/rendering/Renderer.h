#pragma once

namespace nar {
  class Renderer : public Singleton<Renderer> {
   public:
    void RenderFrame();
  };
}