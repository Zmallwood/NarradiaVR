/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ObjectGenerator.h"
#include "world/model/World.h"

namespace nar {
    void ObjectGenerator::GenerateObjects() {
        auto map_area = World::Get()->current_map_area();

        for (auto i = 0; i < 200; i++) {
            auto x = rand() % 100;
            auto y = rand() % 100;

            if (map_area->tiles[x][y].ground != "ground_water")
                map_area->tiles[x][y].object = std::make_shared<Object>("object_tree1");
        }

        for (auto i = 0; i < 200; i++) {
            auto x = rand() % 100;
            auto y = rand() % 100;

            if (map_area->tiles[x][y].ground != "ground_water")
                map_area->tiles[x][y].object = std::make_shared<Object>("object_tree3");
        }

        for (auto i = 0; i < 300; i++) {
            auto x = rand() % 100;
            auto y = rand() % 100;

            if (map_area->tiles[x][y].ground == "ground_grass") {
                if (map_area->tiles[x][y].object == nullptr) {
                    map_area->tiles[x][y].object = std::make_shared<Object>("object_tall_grass6");
                }
            }
        }
    }
}
