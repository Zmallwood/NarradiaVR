#pragma once
#include "BufferTypes.h"
#include "ShaderProgramView.h"

namespace nar {
   class RendererBaseView {
     protected:
      GLuint GenerateNewVertexArrayId();
      GLuint GenerateNewBufferId(BufferTypes buffer_type, GLuint vao_id);
      GLuint GetBufferId(BufferTypes buffer_type, GLuint vao_id);
      void CleanupBase();
      void SetIndicesData(GLuint indices_vbo_id, int num_vertices, const void *data);
      void SetPositions2DData(
          GLuint pos_2d_vb_id, int num_vertices, const void *data, int layout_location = -1);
      void SetPositions3DData(
          GLuint pos_3d_vb_id, int num_vertices, const void *data, int layout_location = -1);
      void
      SetUvsData(GLuint uv_vbo_id, int num_vertices, const void *data, int layout_location = -1);
      void SetColorsData(
          GLuint color_vbo_id, int num_vertices, const void *data, int layout_location = -1);
      void SetNormalsData(
          GLuint normal_vbo_id, int num_vertices, const void *data, int layout_location = -1);
      void UpdateIndicesData(GLuint indices_vbo_id, std::vector<int> &indices);
      void UpdatePositions2DData(
          GLuint positions_vbo_id, std::vector<float> &positions, int layout_location);
      void UpdatePositions3DData(
          GLuint positions_vbo_id, std::vector<float> &positions, int layout_location);
      void UpdateUvsData(GLuint uvs_vbo_id, std::vector<float> &uvs, int layout_location);
      void UpdateColorsData(GLuint colors_vbo_id, std::vector<float> &colors, int layout_location);
      void
      UpdateNormalsData(GLuint normals_vbo_id, std::vector<float> &normals, int layout_location);
      void UseVaoBegin(int vao_id);
      void UseVaoEnd();
      GLuint GetUniformLocation(std::string_view var_name);

      std::shared_ptr<ShaderProgramView> shader_program_view() {
         return shader_program_view_;
      }

      constexpr static int kNumVerticesInRectangle = 4;

     private:
      constexpr static int kNumFloatsPerIndex = 1;
      constexpr static int kNumFloatsPerPosition2D = 2;
      constexpr static int kNumFloatsPerPosition3D = 3;
      constexpr static int kNumFloatsPerColor = 4;
      constexpr static int kNumFloatsPerUv = 2;
      constexpr static int kNumFloatsPerNormal = 3;
      std::vector<GLuint> vao_ids_;
      std::map<BufferTypes, std::map<GLuint, GLuint>> vbo_ids_;
      std::shared_ptr<ShaderProgramView> shader_program_view_ =
          std::make_shared<ShaderProgramView>();
   };
}