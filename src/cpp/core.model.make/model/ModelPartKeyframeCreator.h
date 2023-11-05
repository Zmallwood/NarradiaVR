/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <assimp/scene.h>
#include <vector>
#include <map>

namespace nar
{
    class ModelPartKeyframeCreator : public Ston<ModelPartKeyframeCreator> {
      public:
        aiVectorKey GetPositionKeyframe(const aiScene *, std::string, int) const;

        aiQuatKey GetRotationKeyframe(const aiScene *, std::string, int) const;

        aiVectorKey GetScalingKeyframe(const aiScene *, std::string, int) const;

        auto GetNodePositionKeyframes(const aiScene *, std::string) const
            -> std::vector<aiVectorKey>;

        auto GetNodeRotationKeyframes(const aiScene *, std::string) const -> std::vector<aiQuatKey>;

        auto GetNodeScalingKeyframes(const aiScene *, std::string) const
            -> std::vector<aiVectorKey>;

      private:
        auto GetAnimPositionKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiVectorKey>>;

        auto GetAnimRotationKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiQuatKey>>;

        auto GetAnimScalingKeyframes(const aiScene *) const
            -> std::map<std::string, std::vector<aiVectorKey>>;
    };
}
