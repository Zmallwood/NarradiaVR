#include "Pch.h"
#include "scene_MainScene.h"
#include "engine/core/xr/program/rendering/system_Renderer.h"
#include "matter/struct_Cube.h"
#include "engine/core/xr/program/system_OpenXrProgram.h"
#include "engine/core/xr/common/func_Common.h"
#include "modules/module_ThumbstickMovementModule.h"
#include "engine/core/xr/program/func_Math.Pose.h"
#include "world/world_Player.h"

namespace nar {
   void MainScene::UpdateDerived() {
      GET(ThumbstickMovementModule)->Update();
   }

   void MainScene::RenderDerived() {
      auto predicted_display_time = Renderer::Get()->predicted_display_time();
      // For each locatable space that we want to visualize, render a 100cm cube.
      std::vector<Cube> cubes;

      for (XrSpace visualized_space : OpenXrProgram::Get()->visualized_spaces()) {
         XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
         auto res = xrLocateSpace(
             visualized_space, OpenXrProgram::Get()->app_space(), predicted_display_time,
             &space_location);

         if (XR_UNQUALIFIED_SUCCESS(res)) {
            if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
                (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {

               for (auto y = -5; y <= 5; y++) {
                  for (auto x = -5; x <= 5; x++) {
                     cubes.push_back(Cube{
                         Math::Pose::Translation(
                             {static_cast<float>(x) - GET(Player)->x, -2.0f,
                              static_cast<float>(y) - GET(Player)->y}),
                         {1.0f, 1.0f, 1.0f}});
                  }
               }

               // cubes.push_back(Cube{space_location.pose, {1.0f, 1.0f, 1.0f}});
            }
         }
         else {
            Log::Write(
                Log::Level::Verbose,
                Fmt("Unable to locate a visualized reference space in app space: %d", res));
         }
      }

      // Render a 10cm cube scaled by grab_action for each hand. Note renderHand will only
      // be true when the application has focus.
      for (auto hand : {Side::kLeft, Side::kRight}) {
         XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
         auto res = xrLocateSpace(
             OpenXrProgram::Get()->input().hand_space[hand], OpenXrProgram::Get()->app_space(),
             predicted_display_time, &space_location);

         if (XR_UNQUALIFIED_SUCCESS(res)) {
            if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
                (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
               float scale = 0.1f * OpenXrProgram::Get()->input().hand_scale[hand];
               cubes.push_back(Cube{space_location.pose, {scale, scale, scale}});
            }
         }
         else {
            // Tracking loss is expected when the hand is not active so only log a
            // message if the hand is active.
            if (OpenXrProgram::Get()->input().hand_active[hand] == XR_TRUE) {
               const char *hand_name[] = {"left", "right"};
               Log::Write(
                   Log::Level::Verbose,
                   Fmt("Unable to locate %s hand action space in app space: %d", hand_name[hand],
                       res));
            }
         }
      }

      Renderer::Get()->RenderFrame(cubes);
   }
}