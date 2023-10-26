/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once
#include "system_Options.h"

namespace nar {
   struct Options;

   class OptionsManager : public Singleton<OptionsManager> {
     public:
      void Init();

      std::shared_ptr<Options> options() {
         return options_;
      }

     private:
      void UpdateOptionsFromSystemProperties();

      std::shared_ptr<Options> options_ = std::make_shared<Options>();
   };
}