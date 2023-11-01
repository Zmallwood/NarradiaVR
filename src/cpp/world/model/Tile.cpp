#include "Tile.h"
#include <glm/glm.hpp>

namespace nar {
    void Tile::CalculateNormal(Point3F v0, Point3F v1, Point3F v2) {
        auto point0 = glm::vec3(v0.x, v0.y, v0.z);
        auto point1 = glm::vec3(v1.x, v1.y, v1.z);
        auto Point2 = glm::vec3(v2.x, v2.y, v2.z);
        auto vector0 = point1 - point0;
        auto vector1 = Point2 - point0;
        auto vector2 = glm::cross(vector0, vector1);
        auto normalGl = glm::normalize(vector2);
        normal = {-normalGl.x, -normalGl.y, -normalGl.z};
    }
}
