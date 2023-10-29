/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "MainSceneView.h"
#include "engine/view/RendererView.h"
#include "engine/model/OpenXrProgram.h"
#include "engine/model/InputState.h"
#include "world/model/Player.h"
#include "engine.rendering/view/RendererTilesView.h"
#include "world/model/World.h"

namespace nar {
   MainSceneView::MainSceneView() {
      for (auto y = -15; y < 15; y++) {
         for (auto x = -15; x < 15; x++) {
            rendids_tiles[x][y] = RendererTilesView::Get()->NewTile();
         }
      }
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

      auto gl_code = [=](XrMatrix4x4f vp) {
         auto map_area = World::Get()->current_map_area();

         for (auto y = -15; y < 15; y++) {
            for (auto x = -15; x < 15; x++) {
               Vertex3F v0;
               Vertex3F v1;
               Vertex3F v2;
               Vertex3F v3;

               v0.position = {x * 1.0f, -2.0f, y * 1.0f};
               v1.position = {x * 1.0f + 1.0f, -2.0f, y * 1.0f};
               v2.position = {x * 1.0f + 1.0f, -2.0f, y * 1.0f + 1.0f};
               v3.position = {x * 1.0f, -2.0f, y * 1.0f + 1.0f};

               v0.color = {1.0f, 1.0f, 1.0f, 1.0f};
               v1.color = {1.0f, 1.0f, 1.0f, 1.0f};
               v2.color = {1.0f, 1.0f, 1.0f, 1.0f};
               v3.color = {1.0f, 1.0f, 1.0f, 1.0f};

               v0.normal = {0.0f, 1.0f, 0.0f};
               v1.normal = {0.0f, 1.0f, 0.0f};
               v2.normal = {0.0f, 1.0f, 0.0f};
               v3.normal = {0.0f, 1.0f, 0.0f};

               v0.uv = {0.0f, 0.0f};
               v1.uv = {1.0f, 0.0f};
               v2.uv = {1.0f, 1.0f};
               v3.uv = {0.0f, 1.0f};

               Point3F normal00 = {0.0f, 1.0f, 0.0f};
               Point3F normal10 = {0.0f, 1.0f, 0.0f};
               Point3F normal11 = {0.0f, 1.0f, 0.0f};
               Point3F normal01 = {0.0f, 1.0f, 0.0f};

               auto map_x = x + 50;
               auto map_y = y + 50;

               auto ground = map_area->tiles[map_x][map_y].ground;

               RendererTilesView::Get()->UpdateDrawTile(
                   ground, rendids_tiles[x][y], v0, v1, v2, v3, normal00, normal10, normal11,
                   normal01, vp);
            }
         }
      };

      RendererView::Get()->RenderFrame(gl_code);
   }
}