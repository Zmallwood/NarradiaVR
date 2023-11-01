/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "ShaderProgramView.h"

namespace nar {
   bool ShaderProgramView::Create(const GLchar *vertexShaderSrc, const GLchar *fragShaderSrc) {
      auto success = true;
      program_id_ = glCreateProgram();
      auto vertex_shader_compiled = InitVertexShader(vertexShaderSrc);
      if (vertex_shader_compiled != GL_TRUE) {
         __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Unable to compile vertex shader.");
         success = false;
      }
      else {
         glAttachShader(program_id_, vertex_shader_);
         auto fragment_shader_compiled = InitFragShader(fragShaderSrc);
         if (fragment_shader_compiled != GL_TRUE) {
            __android_log_print(
                ANDROID_LOG_ERROR, "Narradia", "Unable to compile fragment shader.");
            success = false;
         }
         else {
            glAttachShader(program_id_, fragment_shader_);
            glLinkProgram(program_id_);
            GLint program_success = GL_TRUE;
            glGetProgramiv(program_id_, GL_LINK_STATUS, &program_success);
            if (program_success != GL_TRUE) {
               __android_log_print(ANDROID_LOG_ERROR, "Narradia", "Error linking shader program.");
               success = false;
            }
         }
      }
      glDeleteShader(vertex_shader_);
      glDeleteShader(fragment_shader_);
      return success;
   }

   void ShaderProgramView::Cleanup() const {
      glDeleteProgram(program_id_);
   }

   GLuint ShaderProgramView::InitVertexShader(const GLchar *vertexShaderSource) {
      vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex_shader_, 1, &vertexShaderSource, NULL);
      glCompileShader(vertex_shader_);
      GLint vertex_shader_compiled = GL_FALSE;
      glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &vertex_shader_compiled);
      return vertex_shader_compiled;
   }

   GLuint ShaderProgramView::InitFragShader(const GLchar *fragmentShaderSource) {
      fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment_shader_, 1, &fragmentShaderSource, NULL);
      glCompileShader(fragment_shader_);
      GLint fragment_shader_compiled = GL_FALSE;
      glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &fragment_shader_compiled);
      return fragment_shader_compiled;
   }
}
