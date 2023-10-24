#pragma once

namespace nar {
  class VertexCube {
   public:
    VertexCube(float side_length);

    std::vector<Vertex> vertices() {
      return vertices_;
    }

    const std::vector<const unsigned short> indices() {
      return indices_;
    }

   private:
    std::vector<Vertex> vertices_;
    std::vector<const unsigned short> indices_ = {
        // Winding order is clockwise. Each side uses a different color.
        0,  1,  2,  3,  4,  5,  // -X
        6,  7,  8,  9,  10, 11, // +X
        12, 13, 14, 15, 16, 17, // -Y
        18, 19, 20, 21, 22, 23, // +Y
        24, 25, 26, 27, 28, 29, // -Z
        30, 31, 32, 33, 34, 35, // +Z
    };
  };
}