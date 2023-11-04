/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "UtilView.h"

namespace nar {
   XrPosef UtilView::Identity() {
      auto t = XrPosef();
      t.orientation.w = 1;

      return t;
   }

   XrPosef UtilView::Translation(const XrVector3f &translation) {
      XrPosef t = Identity();
      t.position = translation;

      return t;
   }

   XrPosef UtilView::RotateCCWAboutYAxis(float radians, XrVector3f translation) {
      XrPosef t = Identity();
      t.orientation.x = 0.f;
      t.orientation.y = std::sin(radians * 0.5f);
      t.orientation.z = 0.f;
      t.orientation.w = std::cos(radians * 0.5f);
      t.position = translation;

      return t;
   }
}
