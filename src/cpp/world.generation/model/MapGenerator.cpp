/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "MapGenerator.h"
#include "ElevationGenerator.h"
#include "NormalsCalculator.h"
#include "ObjectGenerator.h"
#include "WaterGenerator.h"
#include "world/model/World.h"

namespace nar {
    void MapGenerator::GenerateMapArea() {
        auto map_area = World::Get()->current_map_area();

        for (auto y = 0; y < 100; y++)
            for (auto x = 0; x < 100; x++)
                map_area->tiles[x][y].ground = "ground_grass";

        WaterGenerator::Get()->GenerateWater();
        ElevationGenerator::Get()->GenerateElevation();
        ObjectGenerator::Get()->GenerateObjects();
        NormalsCalculator::Get()->CalculateNormals();
    }
}
