#include "MobGenerator.h"
#include "world/model/World.h"

namespace nar {
    void MobGenerator::GenerateMobs() {
        auto map_area = World::Get()->current_map_area();

        for (auto i = 0; i < 100; i++) {
            auto x = rand() % 100;
            auto y = rand() % 100;

            auto tile = &map_area->tiles[x][y];

            if (tile->mob == nullptr) {
                if (tile->ground != "ground_water") {
                    tile->mob = std::make_shared<Mob>("mob_bird1");
                }
            }
        }
    }
}
