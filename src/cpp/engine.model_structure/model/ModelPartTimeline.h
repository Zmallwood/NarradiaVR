/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "ModelPartKeyframe.h"
#include <memory>
#include <map>

namespace nar
{
    class ModelPartTimeline {
      public:
        std::map<float, std::shared_ptr<const ModelPartKeyframe>> keyframes;
    };
}
