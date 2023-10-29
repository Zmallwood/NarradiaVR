#pragma once
#include "Tile.h"

namespace nar {
   class MapArea {
     public:
      MapArea();

      std::vector<std::vector<Tile>> tiles;
   };
}