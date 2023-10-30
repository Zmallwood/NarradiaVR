#pragma once
#include "ModelPartKeyframe.h"

namespace nar
{
    class ModelPartTimeline {
      public:
        std::map<float, std::shared_ptr<const ModelPartKeyframe>> keyframes;
    };
}