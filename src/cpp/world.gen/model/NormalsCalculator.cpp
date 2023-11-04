#include "NormalsCalculator.h"
#include "../../core/model/Config.h"
#include "../../matter/model/Point.h"
#include "../../world/model/World.h"

namespace nar {
    void NormalsCalculator::CalculateNormals() {
        auto map_area = World::Get()->current_map_area();

        auto elev_amount = Config::Get()->kElevAmount;
        auto tile_size = Config::Get()->kTileSize;

        for (auto y = 0; y < 100; y++) {
            for (auto x = 0; x < 100; x++) {
                auto tile_coord = Point{x, y};
                const auto tile = &map_area->tiles[x][y];
                const auto elev00 = static_cast<float>(tile->elevation);
                auto elev10 = elev00;
                auto elev11 = elev00;
                auto elev01 = elev00;
                const auto coord10 = Point{x + 1, y};
                const auto coord11 = Point{x + 1, y + 1};
                const auto coord01 = Point{x, y + 1};
                if (x + 1 < 100)
                    elev10 = map_area->tiles[x + 1][y].elevation;
                if (x + 1 < 100 && y + 1 < 100)
                    elev11 = map_area->tiles[x + 1][y + 1].elevation;
                if (y + 1 < 100)
                    elev01 = map_area->tiles[x][y + 1].elevation;
                auto x0 = tile_coord.x * tile_size;
                auto y0 = elev00 * elev_amount;
                auto z0 = tile_coord.y * tile_size;
                auto x1 = tile_coord.x * tile_size + tile_size;
                auto y1 = elev10 * elev_amount;
                auto z1 = tile_coord.y * tile_size;
                auto x2 = tile_coord.x * tile_size + tile_size;
                auto y2 = elev11 * elev_amount;
                auto z2 = tile_coord.y * tile_size + tile_size;
                auto x3 = tile_coord.x * tile_size;
                auto y3 = elev01 * elev_amount;
                auto z3 = tile_coord.y * tile_size + tile_size;
                auto p0 = Point3F{x0, y0, z0};
                auto p1 = Point3F{x1, y1, z1};
                auto p3 = Point3F{x3, y3, z3};
                tile->CalculateNormal(p0, p1, p3);
            }
        }
    }
}
