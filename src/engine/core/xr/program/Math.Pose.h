#pragma once

namespace nar {
  namespace Math {
    namespace Pose {
      inline XrPosef Identity() {
        auto t = XrPosef();
        t.orientation.w = 1;

        return t;
      }

      inline XrPosef Translation(const XrVector3f &translation) {
        XrPosef t = Identity();
        t.position = translation;

        return t;
      }

      inline XrPosef RotateCCWAboutYAxis(float radians, XrVector3f translation) {
        XrPosef t = Identity();
        t.orientation.x = 0.f;
        t.orientation.y = std::sin(radians * 0.5f);
        t.orientation.z = 0.f;
        t.orientation.w = std::cos(radians * 0.5f);
        t.position = translation;

        return t;
      }
    }
  }
}