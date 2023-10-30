/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once

namespace nar {
   class ExampleShaderProgramView {
     public:
      void Init();
      void Cleanup();

      GLuint program() {
         return program_;
      }

     private:
      void CheckShader(GLuint shader);
      void CheckProgram(GLuint prog);

      GLuint program_ = 0;
   };
}