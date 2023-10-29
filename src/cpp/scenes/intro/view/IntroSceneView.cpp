/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "IntroSceneView.h"
#include "engine/view/RendererView.h"
#include "engine/model/OpenXrProgram.h"
#include "world/model/Player.h"
#include "engine/model/InputState.h"
#include "engine/model/ModelBank.h"
#include "engine/view/GraphicsGLView.h"

namespace nar {
   void IntroSceneView::Render() {
      auto predicted_display_time = RendererView::Get()->predicted_display_time();
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
                         UtilView::Translation(
                             {static_cast<float>(x) - Player::Get()->x, -2.0f,
                              static_cast<float>(y) - Player::Get()->y}),
                         {1.0f, 1.0f, 1.0f}});
                  }
               }

               // cubes.push_back(Cube{space_location.pose, {1.0f, 1.0f, 1.0f}});
            }
         }
         else {
         }
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

      auto gl_code = [=](XrMatrix4x4f vp) {
         // Set cube primitive data.
         auto vao = GraphicsGLView::Get()->vao();
         auto model_view_projection_uniform_location_ = glGetUniformLocation(
             GraphicsGLView::Get()->shader_program().program(), "ModelViewProjection");
         auto vertex_cube = ModelBank::Get()->vertex_cube();

         glBindVertexArray(vao);

         // Render each cube
         for (const Cube &cube : cubes) {
            // Compute the model-view-projection transform and set it..
            XrMatrix4x4f model;
            XrMatrix4x4f_CreateTranslationRotationScale(
                &model, &cube.pose.position, &cube.pose.orientation, &cube.scale);
            XrMatrix4x4f mvp;
            XrMatrix4x4f_Multiply(&mvp, &vp, &model);
            glUniformMatrix4fv(
                model_view_projection_uniform_location_, 1, GL_FALSE,
                reinterpret_cast<const GLfloat *>(&mvp));

            // Draw the cube.
            glDrawElements(
                GL_TRIANGLES, static_cast<GLsizei>(vertex_cube.indices().size()), GL_UNSIGNED_SHORT,
                nullptr);
         }

         glBindVertexArray(0);
         glUseProgram(0);
      };

      RendererView::Get()->RenderFrame(gl_code);
   }
}