#pragma once
#include "ModelPart.h"

namespace nar 
{
    class Model {
      public:
        Model(int _animDuration);
        const int animDuration = 0;
        std::vector<std::shared_ptr<const ModelPart>> modelParts;
    };
}