/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "Pch.h"
#include "RendererBaseView.h"

namespace nar {
   GLuint RendererBaseView::GenerateNewVertexArrayId() {
      GLuint new_vao_id;
      glGenVertexArrays(1, &new_vao_id);
      vao_ids_.push_back(new_vao_id);
      return new_vao_id;
   }

   GLuint RendererBaseView::GenerateNewBufferId(BufferTypes buffer_type, GLuint vao_id) {
      GLuint new_buffer_id;
      glGenBuffers(1, &new_buffer_id);
      vbo_ids_[buffer_type][vao_id] = new_buffer_id;
      return new_buffer_id;
   }

   GLuint RendererBaseView::GetBufferId(BufferTypes buffer_type, GLuint vao_id) {
      return vbo_ids_.at(buffer_type).at(vao_id);
   }

   void RendererBaseView::CleanupBase() {
      for (auto &buffer_type : vbo_ids_) {
         for (auto &buffer_entry : buffer_type.second)
            glDeleteBuffers(1, &buffer_entry.second);
      }

      for (auto vao_id : vao_ids_) {
         glDeleteVertexArrays(1, &vao_id);
      }

      shader_program_view_->Cleanup();
   }

   void
   RendererBaseView::SetIndicesData(GLuint indices_vbo_id, int num_vertices, const void *data) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
      glBufferData(
          GL_ELEMENT_ARRAY_BUFFER, num_vertices * kNumFloatsPerIndex * sizeof(float), data,
          GL_DYNAMIC_DRAW);
   }

   void RendererBaseView::SetPositions2DData(
       GLuint pos_2d_vb_id, int num_vertices, const void *data, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, pos_2d_vb_id);
      glBufferData(
          GL_ARRAY_BUFFER, num_vertices * kNumFloatsPerPosition2D * sizeof(float), data,
          GL_DYNAMIC_DRAW);
      if (layout_location >= 0) {
         glVertexAttribPointer(
             layout_location, kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
         glEnableVertexAttribArray(layout_location);
      }
   }

   void RendererBaseView::SetPositions3DData(
       GLuint pos_3d_vb_id, int num_vertices, const void *data, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, pos_3d_vb_id);
      glBufferData(
          GL_ARRAY_BUFFER, sizeof(float) * num_vertices * kNumFloatsPerPosition3D, data,
          GL_STATIC_DRAW);
      if (layout_location >= 0) {
         glVertexAttribPointer(
             layout_location, kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
         glEnableVertexAttribArray(layout_location);
      }
   }

   void RendererBaseView::SetUvsData(
       GLuint uv_vbo_id, int num_vertices, const void *data, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_id);
      glBufferData(
          GL_ARRAY_BUFFER, num_vertices * kNumFloatsPerUv * sizeof(float), data, GL_DYNAMIC_DRAW);
      if (layout_location >= 0) {
         glVertexAttribPointer(
             layout_location, kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
         glEnableVertexAttribArray(layout_location);
      }
   }

   void RendererBaseView::SetColorsData(
       GLuint color_vbo_id, int num_vertices, const void *data, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, color_vbo_id);
      glBufferData(
          GL_ARRAY_BUFFER, num_vertices * kNumFloatsPerColor * sizeof(float), data,
          GL_DYNAMIC_DRAW);
      if (layout_location >= 0) {
         glVertexAttribPointer(
             layout_location, kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
         glEnableVertexAttribArray(layout_location);
      }
   }

   void RendererBaseView::SetNormalsData(
       GLuint normal_vbo_id, int num_vertices, const void *data, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_id);
      glBufferData(
          GL_ARRAY_BUFFER, sizeof(float) * num_vertices * kNumFloatsPerNormal, data,
          GL_STATIC_DRAW);
      if (layout_location >= 0) {
         glVertexAttribPointer(
             layout_location, kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
         glEnableVertexAttribArray(layout_location);
      }
   }

   void RendererBaseView::UpdateIndicesData(GLuint indices_vbo_id, std::vector<int> &indices) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * indices.size(), indices.data());
   }

   void RendererBaseView::UpdatePositions2DData(
       GLuint positions_vbo_id, std::vector<float> &positions, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, positions_vbo_id);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
      glVertexAttribPointer(
          layout_location, kNumFloatsPerPosition2D, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
      glEnableVertexAttribArray(layout_location);
   }

   void RendererBaseView::UpdatePositions3DData(
       GLuint positions_vbo_id, std::vector<float> &positions, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, positions_vbo_id);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * positions.size(), positions.data());
      glVertexAttribPointer(
          layout_location, kNumFloatsPerPosition3D, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
      glEnableVertexAttribArray(layout_location);
   }

   void RendererBaseView::UpdateUvsData(
       GLuint uvs_vbo_id, std::vector<float> &uvs, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, uvs_vbo_id);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * uvs.size(), uvs.data());
      glVertexAttribPointer(
          layout_location, kNumFloatsPerUv, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
      glEnableVertexAttribArray(layout_location);
   }

   void RendererBaseView::UpdateColorsData(
       GLuint colors_vbo_id, std::vector<float> &colors, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, colors_vbo_id);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * colors.size(), colors.data());
      glVertexAttribPointer(
          layout_location, kNumFloatsPerColor, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
      glEnableVertexAttribArray(layout_location);
   }

   void RendererBaseView::UpdateNormalsData(
       GLuint normals_vbo_id, std::vector<float> &normals, int layout_location) {
      glBindBuffer(GL_ARRAY_BUFFER, normals_vbo_id);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * normals.size(), normals.data());
      glVertexAttribPointer(
          layout_location, kNumFloatsPerNormal, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
      glEnableVertexAttribArray(layout_location);
   }

   void RendererBaseView::UseVaoBegin(int vao_id) {
      glUseProgram(shader_program_view_->program_id());
      glBindVertexArray(vao_id);
   }

   void RendererBaseView::UseVaoEnd() {
      glBindVertexArray(0);
      glUseProgram(0);
   }

   GLuint RendererBaseView::GetUniformLocation(std::string_view var_name) {
      return glGetUniformLocation(shader_program_view_->program_id(), var_name.data());
   }
}