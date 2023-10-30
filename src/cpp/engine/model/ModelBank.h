/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "FileData.h"

namespace nar {
   class Model;

   /** Loads and provides 3D models used in the rendering.
    */
   class ModelBank : public Singleton<ModelBank> {
     public:
      void LoadModels();

      VertexCube vertex_cube() {
         return vertex_cube_;
      }

     private:
      std::shared_ptr<Model> LoadSingleModel(std::string_view file_name);
      FileData GetAssetData(const char *relative_path);

      VertexCube vertex_cube_ = VertexCube(1.0f);
   };
}