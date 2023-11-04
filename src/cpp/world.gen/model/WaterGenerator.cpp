/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "WaterGenerator.h"
#include "world/model/World.h"

namespace nar {
   void WaterGenerator::GenerateWater() {
      auto map_area = World::Get()->current_map_area();

      for (auto i = 0; i < 15; i++) {
         auto x_center = rand() % 100;
         auto y_center = rand() % 100;
         auto r = 3 + rand() % 4;

         for (auto y = y_center - r; y <= y_center + r; y++) {
            for (auto x = x_center - r; x <= x_center + r; x++) {
               if (x < 0 || y < 0 || x >= 100 || y >= 100)
                  continue;

               auto dx = x - x_center;
               auto dy = y - y_center;

               if (dx * dx + dy * dy <= r * r)
                  map_area->tiles[x][y].ground = "ground_water";
            }
         }
      }
   }
}
