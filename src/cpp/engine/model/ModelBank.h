/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   /** Loads and provides 3D models used in the rendering.
    */
   class ModelBank : public Singleton<ModelBank> {
     public:
      VertexCube vertex_cube() {
         return vertex_cube_;
      }

     private:
      VertexCube vertex_cube_ = VertexCube(1.0f);
   };
}