#include "MobMovementModule.h"
#include "../../../world/model/World.h"
#include <ctime>

namespace nar {
    void MobMovementModule::Update() {
        auto map_area = World::Get()->current_map_area();

        for (auto it = map_area->mobs_mirror.cbegin(); it != map_area->mobs_mirror.cend();) {
            auto mob = it->first;
            auto coord = it->second;

            auto delta_t = static_cast<float>(clock() - mob->ticks_last_movement) / CLOCKS_PER_SEC;

            if (delta_t > 1.0f / mob->movement_speed) {

                auto new_x = coord.x + rand() % 2 - rand() % 2;
                auto new_y = coord.y + rand() % 2 - rand() % 2;

                if (new_x != coord.x || new_y != coord.y) {
                    if (new_x >= 0 && new_y >= 0 && new_x < 100 && new_y < 100) {
                        if (map_area->tiles[new_x][new_y].mob == nullptr) {
                            map_area->tiles[new_x][new_y].mob = mob;
                            map_area->tiles[coord.x][coord.y].mob = nullptr;
                            mob->ticks_last_movement = clock();
                            map_area->mobs_mirror.erase(it++);
                            map_area->mobs_mirror.insert({mob, {new_x, new_y}});
                            continue;
                        }
                    }
                }
            }

            ++it;
        }
    }
}
