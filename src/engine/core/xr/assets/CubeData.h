#pragma once

namespace nar {
  namespace Geometry {
    // Vertices for corners for a 1x1x1 meter cube. (Left/Right, Top/Bottom, Front/Back)
    constexpr XrVector3f kCornerLBB = {-0.5f, -0.5f, -0.5f};
    constexpr XrVector3f kCornerLBF = {-0.5f, -0.5f, 0.5f};
    constexpr XrVector3f kCornerLTB = {-0.5f, 0.5f, -0.5f};
    constexpr XrVector3f kCornerLTF = {-0.5f, 0.5f, 0.5f};
    constexpr XrVector3f kCornerRBB = {0.5f, -0.5f, -0.5f};
    constexpr XrVector3f kCornerRBF = {0.5f, -0.5f, 0.5f};
    constexpr XrVector3f kCornerRTB = {0.5f, 0.5f, -0.5f};
    constexpr XrVector3f kCornerRTF = {0.5f, 0.5f, 0.5f};

#define CUBE_SIDE(V1, V2, V3, V4, V5, V6, COLOR)                                                   \
  {V1, COLOR}, {V2, COLOR}, {V3, COLOR}, {V4, COLOR}, {V5, COLOR}, {V6, COLOR},

    constexpr Vertex c_cube_vertices[] = { // A single side is specified as two triangles = 6 vertices
        CUBE_SIDE(kCornerLTB, kCornerLBF, kCornerLBB, kCornerLTB, kCornerLTF, kCornerLBF, kDarkRed)   // -X
        CUBE_SIDE(kCornerRTB, kCornerRBB, kCornerRBF, kCornerRTB, kCornerRBF, kCornerRTF, kRed)       // +X
        CUBE_SIDE(kCornerLBB, kCornerLBF, kCornerRBF, kCornerLBB, kCornerRBF, kCornerRBB, kDarkGreen) // -Y
        CUBE_SIDE(kCornerLTB, kCornerRTB, kCornerRTF, kCornerLTB, kCornerRTF, kCornerLTF, kGreen)     // +Y
        CUBE_SIDE(kCornerLBB, kCornerRBB, kCornerRTB, kCornerLBB, kCornerRTB, kCornerLTB, kDarkBlue)  // -Z
        CUBE_SIDE(kCornerLBF, kCornerLTF, kCornerRTF, kCornerLBF, kCornerRTF, kCornerRBF, kBlue)      // +Z
    };

    // Winding order is clockwise. Each side uses a different color.
    constexpr unsigned short c_cube_indices[] = {
        0,  1,  2,  3,  4,  5,  // -X
        6,  7,  8,  9,  10, 11, // +X
        12, 13, 14, 15, 16, 17, // -Y
        18, 19, 20, 21, 22, 23, // +Y
        24, 25, 26, 27, 28, 29, // -Z
        30, 31, 32, 33, 34, 35, // +Z
    };
  }
}