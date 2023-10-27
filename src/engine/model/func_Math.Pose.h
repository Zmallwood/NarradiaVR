/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

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