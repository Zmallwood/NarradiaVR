/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "ModelPart.h"
#include <vector>
#include <memory>

namespace nar 
{
    class Model {
      public:
        Model(int _animDuration);
        const int animDuration = 0;
        std::vector<std::shared_ptr<const ModelPart>> modelParts;
    };
}
