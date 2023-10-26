/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

#pragma once

namespace nar {
   class ShaderProgram {
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