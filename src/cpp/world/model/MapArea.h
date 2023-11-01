/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "Tile.h"
#include <vector>

namespace nar {
   class MapArea {
     public:
      MapArea();

      std::vector<std::vector<Tile>> tiles;
   };
}
