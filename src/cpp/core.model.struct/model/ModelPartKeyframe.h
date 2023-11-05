/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <vector>
#include "../../matter/model/Vertex3F.h"

namespace nar
{
    class ModelPartKeyframe {
      public:
        std::vector<Vertex3F> vertices;
    };
}
