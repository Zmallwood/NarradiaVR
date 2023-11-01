/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "MainSceneView.h"
#include "../../../engine.rendering/view/RendererModelsView.h"
#include "../../../engine/model/Config.h"
#include "../../../engine/view/RendererView.h"
#include "engine.rendering/view/RendererTilesView.h"
#include "engine/model/InputState.h"
#include "engine/model/ModelBank.h"
#include "engine/model/OpenXrProgram.h"
#include "world/model/Player.h"
#include "world/model/World.h"
#include <ctime>

namespace nar {
    MainSceneView::MainSceneView() {
        auto map_area = World::Get()->current_map_area();
        auto elev_amount = Config::Get()->kElevAmount;
        auto tile_size = Config::Get()->kTileSize;

        for (auto y = -35; y < 35; y++) {
            for (auto x = -35; x < 35; x++) {
                rendids_tiles[x][y] = RendererTilesView::Get()->NewTile();

                auto map_x = x + 50;
                auto map_y = y + 50;

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

                v0.position = {x * tile_size, -2.0f + elev00, y * tile_size};
                v1.position = {x * tile_size + tile_size, -2.0f + elev10, y * tile_size};
                v2.position = {
                    x * tile_size + tile_size, -2.0f + elev11, y * tile_size + tile_size};
                v3.position = {x * tile_size, -2.0f + elev01, y * tile_size + tile_size};

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
                    rendids_tiles[x][y], v0, v1, v2, v3, normal00, normal10, normal11, normal01);
            }
        }

        auto all_models = ModelBank::Get()->models();
        for (auto &model : all_models)
            RendererModelsView::Get()->NewModel(model.first);
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
        for (auto hand : {Side::kLeft, Side::kRight}) {
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

        auto gl_render_code = [=](XrMatrix4x4f vp) {
            RendererModelsView::Get()->DrawModel(
                "skybox", 0, {0.0f, 0.0f, 0.0f}, vp, 0.0f, 10.0f, 1.0f, {1.0f, 1.0f, 1.0f}, false,
                false, true);

            for (auto y = -35; y < 35; y++) {
                for (auto x = -35; x < 35; x++) {
                    auto map_x = x + 50;
                    auto map_y = y + 50;

                    auto ground = map_area->tiles[map_x][map_y].ground;

                    if (ground == "ground_water") {
                        auto anim_index =
                            ((static_cast<int>(
                                  static_cast<float>(clock()) / CLOCKS_PER_SEC * 1000) +
                              map_x * map_y) %
                             900) /
                            300;

                        if (anim_index > 0)
                            ground = "ground_water_" + std::to_string(anim_index);
                    }

                    RendererTilesView::Get()->DrawTile(ground, rendids_tiles[x][y], vp);

                    auto object = map_area->tiles[map_x][map_y].object;

                    if (nullptr != object) {
                        auto scaling = 1.0f;
                        if (object->type() != "object_tall_grass6")
                            scaling = 2.6f;
                        RendererModelsView::Get()->DrawModel(
                            object->type(), static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
                            {x + 0.5f, -2.0f, y + 0.5f}, vp, 0.0f, scaling, 1.0f);
                    }

                    auto mob = map_area->tiles[map_x][map_y].mob;

                    if (nullptr != mob) {
                        auto mob_y_pos = 0.0f;
                        if (mob->type() == "mob_bird1")
                            mob_y_pos = 3.0f;
                        RendererModelsView::Get()->DrawModel(
                            mob->type(), static_cast<float>(clock()) / CLOCKS_PER_SEC * 10000,
                            {x + 0.5f, -2.0f + mob_y_pos, y + 0.5f}, vp, 0.0f, 0.4f, 1.0f);
                    }
                }
            }
        };
        auto elev_amount = Config::Get()->kElevAmount;
        auto p_x_int = static_cast<int>(Player::Get()->x) + 50;
        auto p_y_int = static_cast<int>(Player::Get()->y) + 50;
        auto elev00 = map_area->tiles[p_x_int][p_y_int].elevation * elev_amount;
        auto elev10 = map_area->tiles[p_x_int + 1][p_y_int].elevation * elev_amount;
        auto elev11 = map_area->tiles[p_x_int + 1][p_y_int + 1].elevation * elev_amount;
        auto elev01 = map_area->tiles[p_x_int][p_y_int + 1].elevation * elev_amount;
        auto dx = Player::Get()->x + 50 - p_x_int;
        auto dy = Player::Get()->y + 50 - p_y_int;
        auto player_elev = elev00 + (elev10 - elev00) * dx + (elev01 - elev00) * dy +
                           (elev11 - elev00) * std::sqrt(dx * dx + dy * dy);

        //        __android_log_print(
        //            ANDROID_LOG_INFO, "Narradia",
        //            "elev: %f, dx: %f, dy: %f, player_x: %f, player_y: %f, x_int: %d, y_int: %d",
        //            player_elev, dx, dy, Player::Get()->x, Player::Get()->y, p_x_int, p_y_int);
        auto player_translation = Point3F{0.0f, player_elev, 0.0f};

        RendererView::Get()->RenderFrame(gl_render_code, player_translation);
    }
}
