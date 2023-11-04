/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "ModelPartTimeline.h"
#include <string_view>
#include <memory>

namespace nar
{
    class ModelPart {
      public:
        ModelPart();
        std::string texName;
        std::unique_ptr<ModelPartTimeline> timeline;
    };
}
