/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "MainSceneView.h"
#include "../../../engine.input_actions/model/InputState.h"
#include "../../../engine.rendering/view/RendererFacelockedView.h"
#include "../../../engine.rendering/view/RendererModelsView.h"
#include "../../../engine.rendering/view/RendererTilesView.h"
#include "../../../engine/model/Config.h"
#include "../../../engine/model/Constants.h"
#include "../../../engine/view/RendererView.h"
#include "../../../world/model/Player.h"
#include "engine.assets/model/ModelBank.h"
#include "engine/model/OpenXrProgram.h"
#include "world/model/World.h"
#include <ctime>

namespace nar {
    MainSceneView::MainSceneView() {
        auto all_models = ModelBank::Get()->models();
        for (auto &model : all_models)
            RendererModelsView::Get()->NewModel(model.first);

        auto tile_size = Config::Get()->kTileSize;
        rendid_facelocked_0_ = RendererFacelockedView::Get()->NewRect();
        rendid_facelocked_1_ = RendererFacelockedView::Get()->NewRect();
        rendid_facelocked_2_ = RendererFacelockedView::Get()->NewRect();
        rendid_facelocked_3_ = RendererFacelockedView::Get()->NewRect();
        Vertex3F v0 = {49.0f*tile_size, 1.0f, 45.0f*tile_size};
        Vertex3F v1 = {51.0f*tile_size, 1.0f, 45.0f*tile_size};
        Vertex3F v2 = {51.0f*tile_size, -2.0f, 45.0f*tile_size};
        Vertex3F v3 = {49.0f*tile_size, -2.0f, 45.0f*tile_size};
        Point3F normal00 = {0.0f, 0.0f, 1.0f};
        Point3F normal10 = {0.0f, 0.0f, 1.0f};
        Point3F normal11 = {0.0f, 0.0f, 1.0f};
        Point3F normal01 = {0.0f, 0.0f, 1.0f};
        v0.color = {1.0f, 1.0f, 1.0f, 1.0f};
        v1.color = {1.0f, 1.0f, 1.0f, 1.0f};
        v2.color = {1.0f, 1.0f, 1.0f, 1.0f};
        v3.color = {1.0f, 1.0f, 1.0f, 1.0f};
        RendererFacelockedView::Get()->SetGeometryRect(
            rendid_facelocked_0_, v0, v1, v2, v3, normal00, normal10, normal11, normal01);

        v0 = {30.0f, 10.0f, 0.0f};
        v1 = {30.0f, 10.0f, 10.0f};
        v2 = {30.0f, -10.0f, 10.0f};
        v3 = {30.0f, -10.0f, 0.0f};
        normal00 = {0.0f, 0.0f, -1.0f};
        normal10 = {0.0f, 0.0f, -1.0f};
        normal11 = {0.0f, 0.0f, -1.0f};
        normal01 = {0.0f, 0.0f, -1.0f};
        RendererFacelockedView::Get()->SetGeometryRect(
            rendid_facelocked_1_, v0, v1, v2, v3, normal00, normal10, normal11, normal01);

        v0 = {-3.0f, 1.0f, 0.0f};
        v1 = {-3.0f, 1.0f, 1.0f};
        v2 = {-3.0f, -1.0f, 1.0f};
        v3 = {-3.0f, -1.0f, 0.0f};
        normal00 = {0.0f, 10.0f, 10.0f};
        normal10 = {0.0f, 10.0f, 10.0f};
        normal11 = {0.0f, 10.0f, 10.0f};
        normal01 = {0.0f, 10.0f, 10.0f};
        RendererFacelockedView::Get()->SetGeometryRect(
            rendid_facelocked_2_, v0, v1, v2, v3, normal00, normal10, normal11, normal01);
        v0 = {0.0f, -10.0f, 0.0f};
        v1 = {0.0f, -10.0f, -10.0f};
        v2 = {0.0f, 10.0f, -10.0f};
        v3 = {0.0f, 10.0f, 0.0f};
        normal00 = {0.0f, 0.0f, 1.0f};
        normal10 = {0.0f, 0.0f, 1.0f};
        normal11 = {0.0f, 0.0f, -1.0f};
        normal01 = {0.0f, 0.0f, -1.0f};
        RendererFacelockedView::Get()->SetGeometryRect(
            rendid_facelocked_3_, v0, v1, v2, v3, normal00, normal10, normal11, normal01);
    }

    void MainSceneView::Render() {
        auto predicted_display_time = RendererView::Get()->predicted_display_time();
        // For each locatable space that we want to visualize, render a 100cm cube.
        std::vector<Cube> cubes;

        for (XrSpace visualized_space : OpenXrProgram::Get()->visualized_spaces()) {
            XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
            auto res = xrLocateSpace(
                visualized_space, OpenXrProgram::Get()->app_space(), predicted_display_time,
                &space_location);

            // if (XR_UNQUALIFIED_SUCCESS(res)) {
            //    if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
            //        (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0)
            //        {

            //       for (auto y = -5; y <= 5; y++) {
            //          for (auto x = -5; x <= 5; x++) {
            //             cubes.push_back(Cube{
            //                 UtilView::Translation(
            //                     {static_cast<float>(x) - Player::Get()->x, -2.0f,
            //                      static_cast<float>(y) - Player::Get()->y}),
            //                 {1.0f, 1.0f, 1.0f}});
            //          }
            //       }

            //       // cubes.push_back(Cube{space_location.pose, {1.0f, 1.0f, 1.0f}});
            //    }
            // }
            // else {
            // }
        }

        // Render a 10cm cube scaled by grab_action for each hand. Note renderHand will only
        // be true when the application has focus.
        for (auto hand : {kLeftHandDevice, kRightHandDevice}) {
            XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
            auto res = xrLocateSpace(
                InputState::Get()->hand_space[hand], OpenXrProgram::Get()->app_space(),
                predicted_display_time, &space_location);

            if (XR_UNQUALIFIED_SUCCESS(res)) {
                if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
                    (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
                    float scale = 0.1f * InputState::Get()->hand_scale[hand];
                    cubes.push_back(Cube{space_location.pose, {scale, scale, scale}});
                }
            }
            else {
                // Tracking loss is expected when the hand is not active so only log a
                // message if the hand is active.
                if (InputState::Get()->hand_active[hand] == XR_TRUE) {
                    const char *hand_name[] = {"left", "right"};
                }
            }
        }

        // RendererView::Get()->RenderFrame(cubes);
        auto map_area = World::Get()->current_map_area();
        auto elev_amount = Config::Get()->kElevAmount;
        auto tile_size = Config::Get()->kTileSize;
        auto p_x_int = static_cast<int>(Player::Get()->x / tile_size);
        auto p_y_int = static_cast<int>(Player::Get()->y / tile_size);
        auto elev00 = map_area->tiles[p_x_int][p_y_int].elevation * elev_amount;
        auto elev10 = map_area->tiles[p_x_int + 1][p_y_int].elevation * elev_amount;
        auto elev11 = map_area->tiles[p_x_int + 1][p_y_int + 1].elevation * elev_amount;
        auto elev01 = map_area->tiles[p_x_int][p_y_int + 1].elevation * elev_amount;
        auto dx = (Player::Get()->x / tile_size - p_x_int);
        auto dy = (Player::Get()->y / tile_size - p_y_int);
        auto player_elev = elev00 + (elev10 - elev00) * dx + (elev01 - elev00) * dy +
                           //(elev11 - elev00) * std::sqrt(dx * dx + dy * dy);
                           0;

        auto cam_dist = 8.0f;
        auto cam_dx =
            -cam_dist * std::cos(glm::radians(-Player::Get()->facing_angle_degrees - 90.0f));
        auto cam_dy =
            -cam_dist * std::sin(glm::radians(-Player::Get()->facing_angle_degrees - 90.0f));

        auto player_translation = Point3F{cam_dx, player_elev + 4.0f, cam_dy};

        auto gl_render_code = [=](XrMatrix4x4f vp) {
            RendererModelsView::Get()->DrawModel(
                "skybox", 0, {Player::Get()->x, Player::Get()->z, Player::Get()->y}, vp, 0.0f,
                10.0f, 1.0f, {1.0f, 1.0f, 1.0f}, false, false, true);

            auto render_distance = 25;

            auto p_x_int2 = static_cast<int>(Player::Get()->x / tile_size);
            auto p_y_int2 = static_cast<int>(Player::Get()->y / tile_size);

            for (auto y = p_y_int2 - render_distance; y < p_y_int2 + render_distance; y++) {
                for (auto x = p_x_int2 - render_distance; x < p_x_int2 + render_distance; x++) {
                    auto map_x = x;
                    auto map_y = y;
                    auto dx = map_x - p_x_int2;
                    auto dy = map_y - p_y_int2;

                    if (map_x < 0 || map_y < 0 || map_x >= 100 || map_y >= 100)
                        continue;

                    if (dx * dx + dy * dy <= render_distance * render_distance)
                        RenderTile(map_x, map_y, vp);
                }
            }
        loops_end:
            auto tile_size = Config::Get()->kTileSize;
            RendererModelsView::Get()->DrawModel(
                "player", static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
                {Player::Get()->x, -2.0f + player_elev, Player::Get()->y}, vp,
                Player::Get()->facing_angle_degrees, 0.7f, 1.0f);
        };

        auto gl_facelocked_render_code = [=](XrMatrix4x4f vp) {
            RendererFacelockedView::Get()->DrawRect("player_tex4", rendid_facelocked_0_, vp);
            RendererFacelockedView::Get()->DrawRect("player_tex4", rendid_facelocked_1_, vp);
            RendererFacelockedView::Get()->DrawRect("player_tex4", rendid_facelocked_2_, vp);
            RendererFacelockedView::Get()->DrawRect("player_tex4", rendid_facelocked_3_, vp);
            auto tile_size = Config::Get()->kTileSize;
//            RendererModelsView::Get()->DrawModel(
//                "player", static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
//                {Player::Get()->x, -2.0f + player_elev, Player::Get()->y}, vp,
//                Player::Get()->facing_angle_degrees, 0.7f, 1.0f);
        };

        auto empty_gl_render_code = [](XrMatrix4x4f vp) {};

        RendererView::Get()->RenderFrame(
            gl_render_code, player_translation, gl_facelocked_render_code);
    }

    void MainSceneView::RenderTile(int map_x, int map_y, XrMatrix4x4f vp) {
        auto map_area = World::Get()->current_map_area();
        auto elev_amount = Config::Get()->kElevAmount;
        auto tile_size = Config::Get()->kTileSize;
        if (rendids_tiles.count(map_x) == 0 || rendids_tiles[map_x].count(map_y) == 0) {
            rendids_tiles[map_x][map_y] = RendererTilesView::Get()->NewTile();

            auto tile = &map_area->tiles[map_x][map_y];

            auto elev00 = tile->elevation * elev_amount;
            auto elev10 = elev00;
            auto elev11 = elev00;
            auto elev01 = elev00;

            auto normal00 = tile->normal;
            auto normal10 = normal00;
            auto normal11 = normal00;
            auto normal01 = normal00;

            if (map_x + 1 < 100) {
                elev10 = map_area->tiles[map_x + 1][map_y].elevation * elev_amount;
                normal10 = map_area->tiles[map_x + 1][map_y].normal;
            }

            if (map_x + 1 < 100 && map_y + 1 < 100) {
                elev11 = map_area->tiles[map_x + 1][map_y + 1].elevation * elev_amount;
                normal11 = map_area->tiles[map_x + 1][map_y + 1].normal;
            }

            if (map_y + 1 < 100) {
                elev01 = map_area->tiles[map_x][map_y + 1].elevation * elev_amount;
                normal01 = map_area->tiles[map_x][map_y + 1].normal;
            }

            Vertex3F v0;
            Vertex3F v1;
            Vertex3F v2;
            Vertex3F v3;

            v0.position = {map_x * tile_size, -2.0f + elev00, map_y * tile_size};
            v1.position = {map_x * tile_size + tile_size, -2.0f + elev10, map_y * tile_size};
            v2.position = {
                map_x * tile_size + tile_size, -2.0f + elev11, map_y * tile_size + tile_size};
            v3.position = {map_x * tile_size, -2.0f + elev01, map_y * tile_size + tile_size};

            v0.color = {1.0f, 1.0f, 1.0f, 1.0f};
            v1.color = {1.0f, 1.0f, 1.0f, 1.0f};
            v2.color = {1.0f, 1.0f, 1.0f, 1.0f};
            v3.color = {1.0f, 1.0f, 1.0f, 1.0f};

            v0.normal = normal00;
            v1.normal = normal10;
            v2.normal = normal11;
            v3.normal = normal01;

            //                v0.normal = {0.0f, 1.0f, 0.0f};
            //                v1.normal = {0.0f, 1.0f, 0.0f};
            //                v2.normal = {0.0f, 1.0f, 0.0f};
            //                v3.normal = {0.0f, 1.0f, 0.0f};

            v0.uv = {0.0f, 0.0f};
            v1.uv = {1.0f, 0.0f};
            v2.uv = {1.0f, 1.0f};
            v3.uv = {0.0f, 1.0f};

            //                Point3F normal00 = {0.0f, 1.0f, 0.0f};
            //                Point3F normal10 = {0.0f, 1.0f, 0.0f};
            //                Point3F normal11 = {0.0f, 1.0f, 0.0f};
            //                Point3F normal01 = {0.0f, 1.0f, 0.0f};

            RendererTilesView::Get()->SetGeometryTile(
                rendids_tiles[map_x][map_y], v0, v1, v2, v3, normal00, normal10, normal11,
                normal01);
        }
        auto elev00 = map_area->tiles[map_x][map_y].elevation * elev_amount;
        auto elev10 = map_area->tiles[map_x + 1][map_y].elevation * elev_amount;
        auto elev11 = map_area->tiles[map_x + 1][map_y + 1].elevation * elev_amount;
        auto elev01 = map_area->tiles[map_x][map_y + 1].elevation * elev_amount;
        auto avg_elev = (elev00 + elev10 + elev11 + elev01) / 4.0f;

        auto ground = map_area->tiles[map_x][map_y].ground;

        if (ground == "ground_water") {
            auto anim_index =
                ((static_cast<int>(static_cast<float>(clock()) / CLOCKS_PER_SEC * 1000) +
                  map_x * map_y) %
                 900) /
                300;

            if (anim_index > 0)
                ground = "ground_water_" + std::to_string(anim_index);
        }

        RendererTilesView::Get()->DrawTile(ground, rendids_tiles[map_x][map_y], vp);

        auto object = map_area->tiles[map_x][map_y].object;

        if (nullptr != object) {
            auto scaling = 1.5f;
            if (object->type() != "object_tall_grass6")
                scaling = 2.6f;
            if (object->type() == "object_wood_wall_n" || object->type() == "object_wood_wall_e" ||
                object->type() == "object_wood_wall_s" || object->type() == "object_wood_wall_w")
                scaling = 0.9f;
            RendererModelsView::Get()->DrawModel(
                object->type(), static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
                {map_x * tile_size + 0.5f * tile_size, -2.0f + avg_elev,
                 map_y * tile_size + 0.5f * tile_size},
                vp, 0.0f, scaling, 1.0f);
        }

        auto mob = map_area->tiles[map_x][map_y].mob;

        if (nullptr != mob) {
            auto mob_y_pos = 0.0f;
            if (mob->type() == "mob_bird1")
                mob_y_pos = 3.0f;
            auto scale = 0.4f;
            RendererModelsView::Get()->DrawModel(
                mob->type(), static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
                {map_x * tile_size + 0.5f * tile_size, -2.0f + avg_elev + mob_y_pos,
                 map_y * tile_size + 0.5f * tile_size},
                vp, 0.0f, scale, 1.0f);
        }
    }
}
