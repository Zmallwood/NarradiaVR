/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
   class UtilView {
     public:
      static XrPosef Identity();
      static XrPosef Translation(const XrVector3f &translation);
      static XrPosef RotateCCWAboutYAxis(float radians, XrVector3f translation);
   };
}
