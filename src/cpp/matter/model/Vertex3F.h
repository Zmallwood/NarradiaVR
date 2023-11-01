/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "Point3F.h"
#include "Color.h"
#include "PointF.h"

namespace nar {
   struct Vertex3F {
      Point3F position;
      Color color;
      PointF uv;
      Point3F normal;
   };
}
