/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#include "ExmplShaderProgView.h"

namespace nar {
   void ExmplShaderProgView::Init() {
      const char *vertex_shader_glsl = // The version statement has come on first line.
#include "ShaderSrcVertex.inc.cpp"
          ;
      const char *fragment_shader_glsl = // The version statement has come on first line.
#include "ShaderSrcFragment.inc.cpp"
          ;

      GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex_shader, 1, &vertex_shader_glsl, nullptr);
      glCompileShader(vertex_shader);
      CheckShader(vertex_shader);

      GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment_shader, 1, &fragment_shader_glsl, nullptr);
      glCompileShader(fragment_shader);
      CheckShader(fragment_shader);

      program_ = glCreateProgram();
      glAttachShader(program_, vertex_shader);
      glAttachShader(program_, fragment_shader);
      glLinkProgram(program_);
      CheckProgram(program_);

      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
   }

   void ExmplShaderProgView::Cleanup() {
      if (program_ != 0)
         glDeleteProgram(program_);
   }

   void ExmplShaderProgView::CheckShader(GLuint shader) {
      GLint r = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &r);

      if (r == GL_FALSE) {
         GLchar msg[4096] = {};
         GLsizei length;
         glGetShaderInfoLog(shader, sizeof(msg), &length, msg);
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Compile shader failed: %s", msg);
         return;
      }
   }

   void ExmplShaderProgView::CheckProgram(GLuint prog) {
      GLint r = 0;
      glGetProgramiv(prog, GL_LINK_STATUS, &r);

      if (r == GL_FALSE) {
         GLchar msg[4096] = {};
         GLsizei length;
         glGetProgramInfoLog(prog, sizeof(msg), &length, msg);
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Link program failed: %s", msg);
         return;
      }
   }
}
