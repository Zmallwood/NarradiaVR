/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "Options.h"

namespace nar {
   struct Options;

   class OptionsManager : public Ston<OptionsManager> {
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
