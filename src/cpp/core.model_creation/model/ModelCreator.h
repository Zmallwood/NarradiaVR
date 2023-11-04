/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

#include "../../core.model_struct/model/Model.h"
#include <assimp/scene.h>
#include <map>
#include <memory>
#include <vector>

namespace nar {
    class ModelPart;

    class ModelCreator : public Ston<ModelCreator> {
      public:
        std::shared_ptr<Model> CreateModel(const aiScene *scene);

      private:
        std::vector<std::shared_ptr<ModelPart>> GetModelParts(const aiScene *scene) const;

        std::map<std::shared_ptr<std::string>, std::vector<int>>
        GetModelMeshIds(const aiScene *scene) const;

        std::vector<aiMesh *>
        GetNodeMeshes(const aiScene *scene, std::vector<int> node_mesh_ids) const;
    };
}
