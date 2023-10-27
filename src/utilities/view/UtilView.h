/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   class UtilView {
     public:
      static XrPosef Identity();
      static XrPosef Translation(const XrVector3f &translation);
      static XrPosef RotateCCWAboutYAxis(float radians, XrVector3f translation);
   };
}