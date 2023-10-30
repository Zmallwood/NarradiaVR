#pragma once
#include "ModelPartTimeline.h"

namespace nar
{
    class ModelPart {
      public:
        ModelPart();
        std::string_view texName = "";
        std::unique_ptr<ModelPartTimeline> timeline;
    };
}