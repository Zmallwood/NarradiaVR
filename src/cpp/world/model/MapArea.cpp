#include "Pch.h"
#include "MapArea.h"

namespace nar {
   MapArea::MapArea() {
      for (auto x = 0; x < 100; x++) {
         tiles.push_back(std::vector<Tile>());

         for (auto y = 0; y < 100; y++) {
            tiles[x].push_back(Tile());
         }
      }
   }
}