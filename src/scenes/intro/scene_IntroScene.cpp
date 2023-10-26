/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "scene_IntroScene.h"
#include "engine/core/input/system_ControllerInput.h"
#include "engine/core/system_SceneManager.h"
#include "engine/core/xr/program/rendering/system_Renderer.h"
#include "engine/core/xr/program/system_OpenXrProgram.h"

namespace nar {
   void IntroScene::UpdateDerived() {
      if (ControllerInput::Get()->right_input_controller()->HasBeenGrabbedPickResult()) {
         SceneManager::Get()->ChangeScene(SceneNames::MainMenu);
      }
   }

   void IntroScene::RenderDerived() {
      auto predicted_display_time = Renderer::Get()->predicted_display_time();
      // For each locatable space that we want to visualize, render a 100cm cube.
      std::vector<Cube> cubes;

      for (XrSpace visualized_space : OpenXrProgram::Get()->visualized_spaces()) {
         XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
         XrResult res = xrLocateSpace(
             visualized_space, OpenXrProgram::Get()->app_space(), predicted_display_time,
             &space_location);

         if (XR_UNQUALIFIED_SUCCESS(res)) {
            if ((space_location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
                (space_location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {
               cubes.push_back(Cube{space_location.pose, {1.0f, 1.0f, 1.0f}});
            }
         }
         else {
         }
      }

      // Render a 10cm cube scaled by grab_action for each hand. Note renderHand will only
      // be true when the application has focus.
      for (auto hand : {Side::kLeft, Side::kRight}) {
         XrSpaceLocation space_location = {XR_TYPE_SPACE_LOCATION};
         XrResult res = xrLocateSpace(
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
            }
         }
      }

      Renderer::Get()->RenderFrame(cubes);
   }
}