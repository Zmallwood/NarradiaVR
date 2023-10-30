/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

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