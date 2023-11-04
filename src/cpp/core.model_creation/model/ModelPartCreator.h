/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

#include "../../matter/model/Point3F.h"
#include <assimp/scene.h>
#include <map>
#include <memory>
#include <vector>

namespace nar {
    class ModelPart;
    class ModelPartKeyframe;

    class ModelPartCreator : public Ston<ModelPartCreator> {
      public:
        auto CreateModelPartFromMesh(const aiScene *, std::string, aiMesh *) const
            -> std::shared_ptr<ModelPart>;

      private:
        std::string GetTexNameForMesh(const aiScene *, aiMesh *) const;
        auto GetNewModelPartKeyframe(
            const aiScene *, std::string, aiMesh *, aiVectorKey, aiQuatKey, aiVectorKey) const
            -> std::shared_ptr<ModelPartKeyframe>;
        std::vector<std::string> GetTexNames(const aiScene *) const;
        aiMatrix4x4 GetNodeTransformation(const aiScene *, std::string) const;
        Point3F GetPosition(aiVector3D, aiMatrix4x4, aiVectorKey, aiQuatKey, aiVectorKey) const;
        auto GetTransformations(const aiScene *) const
            -> std::map<std::shared_ptr<std::string>, aiMatrix4x4>;
        void Translate(Point3F *, aiVectorKey) const;
    };
}
