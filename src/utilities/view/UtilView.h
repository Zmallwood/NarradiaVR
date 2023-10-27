#pragma once

namespace nar {
   class UtilView {
     public:
      static XrPosef Identity();
      static XrPosef Translation(const XrVector3f &translation);
      static XrPosef RotateCCWAboutYAxis(float radians, XrVector3f translation);
   };
}