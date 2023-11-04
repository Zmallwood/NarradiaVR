/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "RendererFacelockedView.h"
#include "core.assets/model/ImageBank.h"
#include "world/model/Player.h"

namespace nar {
   RendererFacelockedView::RendererFacelockedView() {
      const GLchar *vertex_shader_source =
#include "ShaderSrcRendererFacelockedVertex.inc.cpp"
          ;
      const GLchar *fragment_shader_source =
#include "ShaderSrcRendererFacelockedFragment.inc.cpp"
          ;
      shader_program_view()->Create(vertex_shader_source, fragment_shader_source);
      location_projection_ = GetUniformLocation("projection");
      location_view_ = GetUniformLocation("view");
      location_modelViewProjection_ = GetUniformLocation("modelViewProjection");
      location_model_ = GetUniformLocation("model");
      location_alpha_ = GetUniformLocation("mAlpha");
      location_view_pos_ = GetUniformLocation("viewPos");
      location_fog_color_ = GetUniformLocation("fogColor");
   }

   RendererFacelockedView::~RendererFacelockedView() {
      CleanupBase();
   }

   RenderId RendererFacelockedView::NewRect() {
      return NewImagePolygon(4);
   }

   void RendererFacelockedView::SetGeometryRect(
       RenderId vao_id, Vertex3F &v0, Vertex3F &v1, Vertex3F &v2, Vertex3F &v3, Point3F &normal00,
       Point3F &normal10, Point3F &normal11, Point3F &normal01) {
      std::vector<Vertex3F> vertices;
      v0.normal = normal00;
      v1.normal = normal10;
      v2.normal = normal11;
      v3.normal = normal01;
      vertices.push_back(v0);
      vertices.push_back(v1);
      vertices.push_back(v2);
      vertices.push_back(v3);
      SetGeometryImagePolygon(vao_id, vertices);
   }

   void RendererFacelockedView::DrawRect(
       std::string_view image_name, RenderId vao_id, XrMatrix4x4f viewProjectionMatrix,
       bool depth_test_off) {
      DrawImagePolygon(image_name, vao_id, 4, viewProjectionMatrix, depth_test_off);
   }

   void RendererFacelockedView::DrawImagePolygon(
       std::string_view image_name, RenderId vao_id, int vertex_count, XrMatrix4x4f viewProjectionMatrix,
       bool depth_test_off) {
      if (depth_test_off)
         glDisable(GL_DEPTH_TEST);
      else
         glEnable(GL_DEPTH_TEST);
      if (!is_batch_drawing_) {

         auto perspective_matrix =
             glm::perspective(glm::radians(110.0f / 2), 1600.0f / 900.0f, 0.1f, 1000.0f);
         Point3F look_from = {0.0f, 0.0f, 0.0f};
         Point3F look_at = {1.0f, 0.0f, 1.0f};
         auto view_matrix = glm::lookAt(
             glm::vec3(look_from.x, look_from.y, look_from.z),
             glm::vec3(look_at.x, look_at.y, look_at.z), glm::vec3(0.0, 1.0, 0.0));

         glUseProgram(shader_program_view()->program_id());
         // glUniformMatrix4fv(
         //     location_modelViewProjection_, 1, GL_FALSE,
         //     reinterpret_cast<const GLfloat *>(&viewProjectionMatrix));
         XrMatrix4x4f model;
         XrVector3f translation = {-Player::Get()->x, 0.0f, -Player::Get()->y};
         XrQuaternionf rotation = XrQuaternionf();
         XrVector3f scaling = {1.0f, 1.0f, 1.0f};
         XrMatrix4x4f_CreateTranslationRotationScale(&model, &translation, &rotation, &scaling);
         XrMatrix4x4f mvp;
         XrMatrix4x4f_Multiply(&mvp, &viewProjectionMatrix, &model);
         glUniformMatrix4fv(
             location_modelViewProjection_, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&mvp));
         // glUniformMatrix4fv(
         //     location_model_, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&model));
         // glm::mat4 model(1.0);
         // glUniformMatrix4fv(location_model_, 1, GL_FALSE, glm::value_ptr(model));
         glUniform1f(location_alpha_, 1.0f);
         // glm::vec3 view_pos(
         //     Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
         //     Player::Get()->GetSpaceCoord().z);
         glm::vec3 view_pos(0.0f, 0.0f, 0.0f);
         glUniform3fv(location_view_pos_, 1, glm::value_ptr(view_pos));
         // glm::vec3 fog_color_gl(
         //     GraphicsGl::Get()->GetFogColorGround().r, GraphicsGl::Get()->GetFogColorGround().g,
         //     GraphicsGl::Get()->GetFogColorGround().b);
         glm::vec3 fog_color_gl(0.0f, 0.5f, 1.0f);
         glUniform3fv(location_fog_color_, 1, glm::value_ptr(fog_color_gl));
         glUseProgram(shader_program_view()->program_id());
      }
      auto image_id = ImageBank::Get()->GetImage(image_name);
      glBindTexture(GL_TEXTURE_2D, image_id);
      glBindVertexArray(vao_id);
      glDrawElements(GL_TRIANGLE_FAN, vertex_count, GL_UNSIGNED_INT, NULL);
      glBindVertexArray(0);
      if (!is_batch_drawing_)
         glUseProgram(0);
   }

   RenderId RendererFacelockedView::NewImagePolygon(int num_vertices) {
      auto vertex_array_id = GenerateNewVertexArrayId();
      auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vertex_array_id);
      auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vertex_array_id);
      auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vertex_array_id);
      auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vertex_array_id);
      auto normal_buffer_id = GenerateNewBufferId(BufferTypes::Normals, vertex_array_id);
      glUseProgram(shader_program_view()->program_id());
      glBindVertexArray(vertex_array_id);
      SetIndicesData(index_buffer_id, num_vertices, nullptr);
      SetPositions3DData(position_buffer_id, num_vertices, nullptr);
      SetColorsData(color_buffer_id, num_vertices, nullptr);
      SetUvsData(uv_buffer_id, num_vertices, nullptr);
      SetNormalsData(normal_buffer_id, num_vertices, nullptr);
      glBindVertexArray(0);
      glUseProgram(0);
      return vertex_array_id;
   }

   void
   RendererFacelockedView::SetGeometryImagePolygon(RenderId vao_id, std::vector<Vertex3F> &vertices) {
      if (!is_batch_drawing_)
         glUseProgram(shader_program_view()->program_id());
      std::vector<int> indices(vertices.size());
      std::iota(std::begin(indices), std::end(indices), 0);
      std::vector<float> positions;
      std::vector<float> colors;
      std::vector<float> uvs;
      std::vector<float> normals;
      for (auto &vertex : vertices) {
         positions.push_back(vertex.position.x);
         positions.push_back(vertex.position.y);
         positions.push_back(vertex.position.z);
         colors.push_back(vertex.color.r);
         colors.push_back(vertex.color.g);
         colors.push_back(vertex.color.b);
         colors.push_back(vertex.color.a);
         uvs.push_back(vertex.uv.x);
         uvs.push_back(vertex.uv.y);
         auto vertex_normal = vertex.normal;
         normals.push_back(vertex_normal.x);
         normals.push_back(vertex_normal.y);
         normals.push_back(vertex_normal.z);
      }
      auto index_buffer_id = GetBufferId(BufferTypes::Indices, vao_id);
      auto position_buffer_id = GetBufferId(BufferTypes::Positions, vao_id);
      auto color_buffer_id = GetBufferId(BufferTypes::Colors, vao_id);
      auto uv_buffer_id = GetBufferId(BufferTypes::Uvs, vao_id);
      auto normal_buffer_id = GetBufferId(BufferTypes::Normals, vao_id);
      glBindVertexArray(vao_id);
      UpdateIndicesData(index_buffer_id, indices);
      UpdatePositions3DData(position_buffer_id, positions, kLocationPosition);
      UpdateColorsData(color_buffer_id, colors, kLocationColor);
      UpdateUvsData(uv_buffer_id, uvs, kLocationUv);
      UpdateNormalsData(normal_buffer_id, normals, kLocationNormal);
      glBindVertexArray(0);
      if (!is_batch_drawing_)
         glUseProgram(0);
   }
}
