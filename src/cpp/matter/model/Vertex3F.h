/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar {
   struct Vertex3F {
      Point3F position;
      Color color;
      PointF uv;
      Point3F normal;
   };
}