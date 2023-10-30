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
      std::shared_ptr<Model> GetModel(std::string_view model_name);

      std::map<std::string_view, std::shared_ptr<Model>> models() {
         return models_;
      }

      VertexCube vertex_cube() {
         return vertex_cube_;
      }

     private:
      void LoadSingleModel(std::string_view file_name);
      FileData GetAssetData(const char *relative_path);

      std::map<std::string_view, std::shared_ptr<Model>> models_;
      VertexCube vertex_cube_ = VertexCube(1.0f);
   };
}