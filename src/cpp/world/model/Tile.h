/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../matter/model/Point3F.h"
#include "Object.h"

namespace nar {
    class Tile {
      public:
        void CalculateNormal(Point3F v0, Point3F v1, Point3F v2);

        std::string_view ground = "";
        float elevation = 0.0f;
        std::shared_ptr<Object> object = nullptr;
        Point3F normal = {0.0f, 1.0f, 0.0f};
    };
}
