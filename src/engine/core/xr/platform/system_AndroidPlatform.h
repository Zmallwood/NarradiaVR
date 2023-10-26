/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   struct Options;

   class AndroidPlatform : public Singleton<AndroidPlatform> {
     public:
      AndroidPlatform();
      std::vector<std::string> GetInstanceExtensions() const;
      XrBaseInStructure *GetInstanceCreateExtension() const;

      XrInstanceCreateInfoAndroidKHR instance_create_info_android_;
   };
}