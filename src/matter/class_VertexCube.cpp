/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#include "Pch.h"
#include "class_VertexCube.h"

#define CUBE_SIDE(V1, V2, V3, V4, V5, V6, COLOR)                                                   \
   Vertex{V1, COLOR}, Vertex{V2, COLOR}, Vertex{V3, COLOR}, Vertex{V4, COLOR}, Vertex{V5, COLOR},  \
       Vertex {                                                                                    \
      V6, COLOR                                                                                    \
   }

namespace nar {
   VertexCube::VertexCube(float side_length) {
      auto half_length = side_length / 2.0f;
      XrVector3f kCornerLBB = {-half_length, -half_length, -half_length}; // Left-bottom-back
      XrVector3f kCornerLBF = {-half_length, -half_length, half_length};  // Left-bottom-front
      XrVector3f kCornerLTB = {-half_length, half_length, -half_length};  // Left-top-back
      XrVector3f kCornerLTF = {-half_length, half_length, half_length};   // Left-top-front
      XrVector3f kCornerRBB = {half_length, -half_length, -half_length};  // Right-bottom-back
      XrVector3f kCornerRBF = {half_length, -half_length, half_length};   // Right-bottom-front
      XrVector3f kCornerRTB = {half_length, half_length, -half_length};   // Right-top-back
      XrVector3f kCornerRTF = {half_length, half_length, half_length};    // Right-top-front

      vertices_ = std::vector<Vertex>{
          /* A single side is specified as two triangles = 6 vertices, and one color.
           */
          CUBE_SIDE(
              kCornerLTB, kCornerLBF, kCornerLBB, kCornerLTB, kCornerLTF, kCornerLBF,
              kDarkRed), // -X
          CUBE_SIDE(
              kCornerRTB, kCornerRBB, kCornerRBF, kCornerRTB, kCornerRBF, kCornerRTF, kRed), // +X
          CUBE_SIDE(
              kCornerLBB, kCornerLBF, kCornerRBF, kCornerLBB, kCornerRBF, kCornerRBB,
              kDarkGreen), // -Y
          CUBE_SIDE(
              kCornerLTB, kCornerRTB, kCornerRTF, kCornerLTB, kCornerRTF, kCornerLTF, kGreen), // +Y
          CUBE_SIDE(
              kCornerLBB, kCornerRBB, kCornerRTB, kCornerLBB, kCornerRTB, kCornerLTB,
              kDarkBlue), // -Z
          CUBE_SIDE(
              kCornerLBF, kCornerLTF, kCornerRTF, kCornerLBF, kCornerRTF, kCornerRBF, kBlue) // +Z
      };
   }
}