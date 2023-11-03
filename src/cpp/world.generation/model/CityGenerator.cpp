#include "CityGenerator.h"
#include "world/model/World.h"

namespace nar {
    void CityGenerator::GenerateCity() {
        auto map_area = World::Get()->current_map_area();

        // clang-format off
        std::vector<Point> predef_nodes = {
            // 0         1          2          3         4         5        6
            {26, 26}, {34, 26},  {42, 26},  {50,26}, {58, 26}, {66, 26}, {74, 26},
            //  7        8           9         10       11         12        13
            {26, 34}, {34, 34},  {42, 34},  {50,34}, {58, 34}, {66, 34}, {74, 34},
            //  14       15         16          17       18        19        20
            {26, 42}, {34, 42},  {42, 42},  {50,42}, {58, 42}, {66, 42}, {74, 42},
            //  21       22          23         24      25         26        27
            {26, 50}, {34, 50},  {42, 50},  {50,50}, {58, 50}, {66, 50}, {74, 50},
            //  28       29          30        31        32        33        34
            {26, 58}, {34, 58},  {42, 58},  {50,58}, {58, 58}, {66, 58}, {74, 58},
            //  35       36          37         38       39        40        41
            {26, 66}, {34, 66},  {42, 66},  {50,66}, {58, 66}, {66, 66}, {74, 66},
            //   42      43          44        45        46       47         48
            {26, 74}, {34, 74},  {42, 74},  {50,74}, {58, 74}, {66, 74}, {74, 74}
        };

        enum class Dir{None, N, E, S, W};

        struct House
        {
            Point center;
            int closest_node_index;
            Dir door_direction = Dir::None;
        };

        std::vector<House> predef_houses = {
            {{30, 30},8, Dir::S}, {{38,30},8, Dir::S}, {{46, 30},10,Dir::E}, {{54, 30}, 4, Dir::E}, {{62, 30}, 11, Dir::S}, {{70, 30}, 12, Dir::W}, 
            {{30, 38},14, Dir::S}, {{38,38},15,Dir::S}, {{46, 38},9, Dir::W}, {{54, 38}, 18, Dir::E}, {{62, 38}, 18, Dir::W}, {{70, 38}, 12, Dir::W}, 
            {{30, 46},22, Dir::E}, {{38,46},15, Dir::W}, {{46, 46},17, Dir::E}, {{54, 46}, 25, Dir::S}, {{62, 46}, 26, Dir::S}, {{70, 46}, 20, Dir::N}, 
            {{30, 54},29, Dir::S}, {{38,54},23, Dir::E}, {{46, 54},31, Dir::S}, {{54, 54}, 24, Dir::N}, {{62, 54}, 33, Dir::E}, {{70, 54}, 33, Dir::W}, 
            {{30, 62},29, Dir::N}, {{38,62},36, Dir::W}, {{46, 62},38, Dir::E}, {{54, 62}, 38, Dir::W}, {{62, 62}, 32, Dir::N}, {{70, 62}, 34, Dir::N}, 
            {{30, 70},43, Dir::E}, {{38,70},36, Dir::N}, {{46, 70},45, Dir::E}, {{54, 70}, 45, Dir::W}, {{62, 70}, 39, Dir::N}, {{70, 70}, 41, Dir::N}
        };
        // clang-format on

        auto num_houses = 8;
        std::vector<Point> used_nodes;
        std::vector<House> used_houses;

        for (auto i = 0; i < num_houses; i++) {
            auto index = rand() % predef_houses.size();
            auto house_entry = predef_houses.at(index);
            used_houses.push_back(house_entry);
            auto entrance = house_entry.closest_node_index;
            auto entrance_node = predef_nodes.at(entrance);
            used_nodes.push_back(entrance_node);
        }

        auto num_nodes = 30;

        for (auto i = 0; i < num_nodes; i++) {
            auto index = rand() % predef_nodes.size();
            auto pos = predef_nodes.at(index);
            used_nodes.push_back(pos);
        }

        for (auto y = 26; y <= 74; y++) {
            for (auto x = 25; x <= 74; x++) {
                map_area->tiles[x][y].ground = "ground_grass";
                map_area->tiles[x][y].elevation = 0;
            }
        }

        for (auto node : used_nodes) {

            auto dx = node.x - 50;
            auto dy = node.y - 50;
            auto x_curr = 50;
            auto y_curr = 50;

            if (dx > 0 && dy < 0) {
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    map_area->tiles[x_curr][y_curr].object = nullptr;
                    y_curr--;
                }
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    map_area->tiles[x_curr][y_curr].object = nullptr;
                    x_curr++;
                }
            }

            if (dx > 0 && dy > 0) {
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    map_area->tiles[x_curr][y_curr].object = nullptr;
                    x_curr++;
                }
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    map_area->tiles[x_curr][y_curr].object = nullptr;
                    y_curr++;
                }
            }

            if (dx < 0 && dy > 0) {
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    y_curr++;
                }
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    x_curr--;
                }
            }

            if (dx < 0 && dy < 0) {
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    x_curr--;
                }
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[x_curr][y_curr].ground = "ground_cobblestone";
                    y_curr--;
                }
            }
        }

        for (auto entry : used_houses) {

            auto w = 3 + rand() % 4;
            auto h = 3 + rand() % 4;

            for (auto y = entry.center.y - h/ 2; y < entry.center.y + h/ 2; y++) {
                for (auto x = entry.center.x - w/ 2; x < entry.center.x + w/ 2; x++) {
                    map_area->tiles[x][y].ground = "ground_wood_floor";
                    map_area->tiles[x][y].object = nullptr;
                }
            }

            continue;
            Point door;

            switch (entry.door_direction) {
            case Dir::N:
                door = {entry.center.x, entry.center.y - h/ 2 - 1};
                break;
            case Dir::E:
                door = {entry.center.x + w/ 2 + 1, entry.center.y};
                break;
            case Dir::S:
                door = {entry.center.x, entry.center.y + h/ 2 + 1};
                break;
            case Dir::W:
                door = {entry.center.x - w/ 2 - 1, entry.center.y};
                break;
            }

            auto closest_node = used_nodes.at(entry.closest_node_index);

            auto dx = closest_node.x - door.x;
            auto dy = closest_node.y - door.y;

            auto absdx = std::abs(dx);
            auto absdy = std::abs(dy);

            auto normx = 0;
            auto normy = 0;

            if (dx)
                normx = dx / absdx;

            if (dy)
                normy = dy / absdy;

            auto curr_x = door.x;
            auto curr_y = door.y;

            if (dx < dy) {
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[curr_x][curr_y].ground = "ground_cobblestone";
                    map_area->tiles[curr_x][curr_y].object = nullptr;
                    curr_x += normx;
                }
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[curr_x][curr_y].ground = "ground_cobblestone";
                    map_area->tiles[curr_x][curr_y].object = nullptr;
                    curr_y += normy;
                }
            }
            else {
                for (auto y = 0; y < std::abs(dy); y++) {
                    map_area->tiles[curr_x][curr_y].ground = "ground_cobblestone";
                    map_area->tiles[curr_x][curr_y].object = nullptr;
                    curr_y += normy;
                }
                for (auto x = 0; x < std::abs(dx); x++) {
                    map_area->tiles[curr_x][curr_y].ground = "ground_cobblestone";
                    map_area->tiles[curr_x][curr_y].object = nullptr;
                    curr_x += normx;
                }
            }
        }
    }
}
