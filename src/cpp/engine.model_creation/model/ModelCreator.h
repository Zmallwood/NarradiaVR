/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <memory>
#include <vector>
#include <map>
#include "../../engine.model_structure/model/Model.h"
#include <assimp/scene.h>

namespace nar {
   class ModelPart;

   class ModelCreator : public Singleton<ModelCreator> {
     public:
      auto CreateModel(const aiScene *) -> std::shared_ptr<Model>;

     private:
      auto GetModelParts(const aiScene *) const -> std::vector<std::shared_ptr<ModelPart>>;
      auto GetModelMeshIds(const aiScene *) const
          -> std::map<std::shared_ptr<std::string>, std::vector<int>>;
      auto GetNodeMeshes(const aiScene *, std::vector<int>) const -> std::vector<aiMesh *>;
   };
}
