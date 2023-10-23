#pragma once

namespace nar {
  class Renderer : public Singleton<Renderer> {
   public:
    void RenderFrame();

   private:
    bool RenderLayer(
        XrTime predicted_display_time,
        std::vector<XrCompositionLayerProjectionView> &projection_layer_views,
        XrCompositionLayerProjection &layer);
  };
}