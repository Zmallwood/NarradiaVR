/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include <GLES2/gl2.h>

namespace nar {
   class ShaderProgView {
     public:
      bool Create(const GLchar *, const GLchar *);
      void Cleanup() const;

      GLuint program_id() {
         return program_id_;
      }

     private:
      GLuint InitVertexShader(const GLchar *);
      GLuint InitFragShader(const GLchar *);

      GLuint vertex_shader_ = 0;
      GLuint fragment_shader_ = 0;
      GLuint program_id_ = 0;
   };
}
