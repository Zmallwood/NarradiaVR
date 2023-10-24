#pragma once

namespace nar {
  class ModelBank : public Singleton<ModelBank> {
   public:
    VertexCube vertex_cube() {
      return vertex_cube_;
    }

   private:
    VertexCube vertex_cube_ = VertexCube(1.0f);
  };
}