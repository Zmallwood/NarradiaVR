/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#include "RendererModelsView.h"
#include "../../engine/model/ModelBank.h"
#include "engine.model_structure/model/Model.h"
#include "engine/model/ImageBank.h"
#include "world/model/Player.h"
#include <GLES2/gl2.h>

namespace nar {
    RendererModelsView::RendererModelsView() {
        const GLchar *vertex_shader_source =
#include "ShaderSourceRendererModelsVertex.inc.cpp"
            ;
        const GLchar *fragment_shader_source =
#include "ShaderSourceRendererModelsFragment.inc.cpp"
            ;
        shader_program_view()->Create(vertex_shader_source, fragment_shader_source);
        location_view_ = GetUniformLocation("view");
        location_projection_ = GetUniformLocation("projection");
        location_alpha_ = GetUniformLocation("mAlpha");
        location_model_ = GetUniformLocation("model");
        location_model_no_translation_ = GetUniformLocation("modelNoTranslation");
        location_viewProjection_ = GetUniformLocation("viewProjection");
        location_color_mod_ = GetUniformLocation("mColorMod");
        location_view_pos_ = GetUniformLocation("viewPos");
        location_fog_color_ = GetUniformLocation("fogColor");
        location_no_fog_ = GetUniformLocation("noFog");
        location_no_lighting_ = GetUniformLocation("noLighting");
    }

    void RendererModelsView::NewModel(std::string_view model_name) {
        auto model = ModelBank::Get()->GetModel(model_name);
        model_ids_.insert({model_name, std::map<int, std::map<float, const BodyData>>()});
        auto i_body = 0;
        for (auto &body : model->modelParts) {
            auto &timelines = body->timeline->keyframes;
            for (auto &keyframe : timelines) {
                auto keyframe_time = keyframe.first;
                auto anim_key_body_keyframe = keyframe.second;
                auto vertex_count = anim_key_body_keyframe->vertices.size();
                auto body_keyframe_id = NewBodyKeyframe(model_name, keyframe_time, vertex_count);
                if (model_ids_.at(model_name).count(i_body) == 0)
                    model_ids_.at(model_name).insert({i_body, std::map<float, const BodyData>()});
                BodyData body_data;
                body_data.rendid = body_keyframe_id;
                body_data.image_name = body->texName;
                body_data.num_vertices = vertex_count;
                model_ids_.at(model_name).at(i_body).insert({keyframe_time, body_data});
                auto &model_keyframe = body->timeline->keyframes.at(keyframe_time);
                std::vector<Vertex3F> vertices;
                std::vector<Point3F> normals;
                auto &anim_vertices = model_keyframe->vertices;
                for (auto v : anim_vertices) {
                    Vertex3F v3f;
                    v3f.position = v.position;
                    v3f.color = v.color;
                    v3f.uv = v.uv;
                    vertices.push_back(v3f);
                    Point3F n3f;
                    n3f.x = v.normal.x;
                    n3f.y = v.normal.y;
                    n3f.z = v.normal.z;
                    normals.push_back(n3f);
                }
                NewBodyKeyframeGeometry(
                    body->texName, keyframe_time, body_keyframe_id, vertices, normals);
            }
            i_body++;
        }
    }

    RenderId RendererModelsView::NewBodyKeyframe(
        std::string_view model_name, float msTime, int numVertices) {
        auto vao_id = GenerateNewVertexArrayId();
        if (timelines_.count(model_name) == 0)
            timelines_.insert({model_name, std::map<float, RenderId>()});
        timelines_.at(model_name).insert({msTime, vao_id});
        return vao_id;
    }

    void RendererModelsView::NewBodyKeyframeGeometry(
        std::string_view model_name, float msTime, RenderId vaoId, std::vector<Vertex3F> vertices,
        std::vector<Point3F> vertexNormals) {
        glEnable(GL_DEPTH_TEST);
        UseVaoBegin(vaoId);
        //      glUniformMatrix4fv(
        //          location_projection_, 1, GL_FALSE,
        //          value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
        //      glUniformMatrix4fv(location_view_, 1, GL_FALSE,
        //      value_ptr(CameraGl::Get()->GetViewMatrix())); glUniform1f(location_alpha_, 1.0f);
        //      auto image_id = ImageBank::Get()->GetImage(model_name); glBindTexture(GL_TEXTURE_2D,
        //      image_id);
        std::vector<int> indices(vertices.size());
        std::iota(std::begin(indices), std::end(indices), 0);
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> uvs;
        std::vector<float> normals;
        auto i = 0;
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
            if (vertexNormals.size() > i) {
                auto vertexNormal = vertexNormals.at(i);
                normals.push_back(vertexNormal.x);
                normals.push_back(-vertexNormal.y);
                normals.push_back(vertexNormal.z);
            }
            i++;
        }
        auto index_buffer_id = GenerateNewBufferId(BufferTypes::Indices, vaoId);
        auto position_buffer_id = GenerateNewBufferId(BufferTypes::Positions, vaoId);
        auto color_buffer_id = GenerateNewBufferId(BufferTypes::Colors, vaoId);
        auto uv_buffer_id = GenerateNewBufferId(BufferTypes::Uvs, vaoId);
        auto normal_buffer_id = GenerateNewBufferId(BufferTypes::Normals, vaoId);
        auto num_vertices = vertices.size();
        SetIndicesData(index_buffer_id, num_vertices, indices.data());
        SetPositions3DData(position_buffer_id, num_vertices, positions.data(), kLocationPosition);
        SetColorsData(color_buffer_id, num_vertices, colors.data(), kLocationColor);
        SetUvsData(uv_buffer_id, num_vertices, uvs.data(), kLocationUv);
        SetNormalsData(normal_buffer_id, num_vertices, normals.data(), kLocationNormal);
        UseVaoEnd();
    }

    void RendererModelsView::StartBatchDrawing(XrMatrix4x4f viewProjectionMatrix) {
        is_batch_drawing_ = true;
        glEnable(GL_DEPTH_TEST);
        glUseProgram(shader_program_view()->program_id());
        //      auto perspective_matrix =
        //          glm::perspective(glm::radians(110.0f / 2), 1600.0f / 900.0f, 0.1f, 1000.0f);
        //      Point3F look_from = {0.0f, 0.0f, 0.0f};
        //      Point3F look_at = {1.0f, 0.0f, 1.0f};
        //      auto view_matrix = glm::lookAt(
        //          glm::vec3(look_from.x, look_from.y, look_from.z),
        //          glm::vec3(look_at.x, look_at.y, look_at.z), glm::vec3(0.0, 1.0, 0.0));
        //      glUniformMatrix4fv(location_projection_, 1, GL_FALSE,
        //      value_ptr(perspective_matrix)); glUniformMatrix4fv(location_view_, 1, GL_FALSE,
        //      glm::value_ptr(view_matrix));
        XrMatrix4x4f model;
        XrVector3f translation = {0.0f, 0.0f, 0.0f};
        XrQuaternionf rotation = XrQuaternionf();
        XrVector3f scaling = {1.0f, 1.0f, 1.0f};
        XrMatrix4x4f_CreateTranslationRotationScale(&model, &translation, &rotation, &scaling);
        XrMatrix4x4f mvp;
        XrMatrix4x4f_Multiply(&mvp, &viewProjectionMatrix, &model);
        glUniformMatrix4fv(
            location_viewProjection_, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&mvp));
    }

    void RendererModelsView::StopBatchDrawing() {
        is_batch_drawing_ = false;
    }

    void RendererModelsView::DrawModel(
        std::string_view model_name, float msTime, Point3F position,
        XrMatrix4x4f viewProjectionMatrix, float rotation, float scaling, float brightness,
        glm::vec3 colorMod, bool noFog, bool noLighting) const {
        // glDisable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        if (model_ids_.count(model_name) == 0)
            return;
        if (!is_batch_drawing_) {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(shader_program_view()->program_id());
            //         auto perspective_matrix =
            //             glm::perspective(glm::radians(110.0f / 2), 1600.0f / 900.0f, 0.1f,
            //             1000.0f);
            //         Point3F look_from = {0.0f, 0.0f, 0.0f};
            //         Point3F look_at = {1.0f, 0.0f, 1.0f};
            //         auto view_matrix = glm::lookAt(
            //             glm::vec3(look_from.x, look_from.y, look_from.z),
            //             glm::vec3(look_at.x, look_at.y, look_at.z), glm::vec3(0.0, 1.0, 0.0));
            //         glUniformMatrix4fv(location_projection_, 1, GL_FALSE,
            //         value_ptr(perspective_matrix)); glUniformMatrix4fv(location_view_, 1,
            //         GL_FALSE, glm::value_ptr(view_matrix));
            XrMatrix4x4f model;
            XrVector3f translation = {0.0f, 0.0f, 0.0f};
            XrQuaternionf rotation = XrQuaternionf();
            XrVector3f scaling = {1.0f, 1.0f, 1.0f};
            XrMatrix4x4f_CreateTranslationRotationScale(&model, &translation, &rotation, &scaling);
            XrMatrix4x4f mvp;
            XrMatrix4x4f_Multiply(&mvp, &viewProjectionMatrix, &model);
            glUniformMatrix4fv(
                location_viewProjection_, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&mvp));
            //         glUniformMatrix4fv(
            //             location_projection_, 1, GL_FALSE,
            //             value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            //         auto &view_matrix = CameraGl::Get()->GetViewMatrix();
            //         glUniformMatrix4fv(location_view_, 1, GL_FALSE, glm::value_ptr(view_matrix));
        }
        auto model_matrix = glm::rotate(
            glm::scale(
                glm::translate(
                    glm::mat4(1.0),
                    glm::vec3(
                        position.x - Player::Get()->x, position.y, position.z - Player::Get()->y)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        auto model_no_translation_matrix = glm::rotate(
            glm::scale(
                glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                glm::vec3(scaling, scaling, scaling)),
            glm::radians(rotation), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(location_model_, 1, GL_FALSE, glm::value_ptr(model_matrix));
        glUniformMatrix4fv(
            location_model_no_translation_, 1, GL_FALSE,
            glm::value_ptr(model_no_translation_matrix));
        glUniform3fv(location_color_mod_, 1, glm::value_ptr(colorMod));
        //      glm::vec3 viewPos(
        //          Player::Get()->GetSpaceCoord().x, Player::Get()->GetSpaceCoord().y,
        //          Player::Get()->GetSpaceCoord().z);
        glm::vec3 viewPos(0.0f, 0.0f, 0.0f);
        glUniform3fv(location_view_pos_, 1, glm::value_ptr(viewPos));
        glm::vec3 fogColorGl(0.0f, 0.5f, 1.0f);
        glUniform3fv(location_fog_color_, 1, glm::value_ptr(fogColorGl));
        glUniform1f(location_alpha_, brightness);
        glUniform1f(location_no_fog_, noFog ? 1.0f : 0.0f);
        glUniform1f(location_no_lighting_, noLighting ? 1.0f : 0.0f);
        auto &all_nodes = model_ids_.at(model_name);
        auto p_model = ModelBank::Get()->GetModel(model_name);
        int ms_time_used;
        if (p_model->animDuration == 0)
            ms_time_used = 0;
        else
            ms_time_used =
                static_cast<int>(msTime * global_animation_speed_) % p_model->animDuration;
        auto &model_data = all_nodes;
        for (auto &entry : all_nodes) {
            auto &timeline = entry.second;
            auto found_time = -1.0f;
            const BodyData *p_body_data = nullptr;
            for (auto &keyframe : timeline) {
                auto time = keyframe.first;
                if (ms_time_used >= time)
                    found_time = time;
            }
            auto &body_data = timeline.at(found_time);
            glBindVertexArray(body_data.rendid);
            auto image_id = ImageBank::Get()->GetImage(body_data.image_name);
            // auto image_id = ImageBank::Get()->GetImage("object_tree1_tex0");
            glBindTexture(GL_TEXTURE_2D, image_id);
            glDrawElements(GL_TRIANGLES, body_data.num_vertices, GL_UNSIGNED_INT, NULL);
        }
        glBindVertexArray(0);
        if (!is_batch_drawing_)
            glUseProgram(0);
        // glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);
    }

    void RendererModelsView::DrawModelsMany(
        std::string_view model_name, float msTime, std::vector<Point3F> positions,
        std::vector<float> rotations, std::vector<float> scalings, std::vector<float> brightnesses,
        std::vector<glm::vec3> colorMods, XrMatrix4x4f viewProjectionMatrix) const {
        if (model_ids_.count(model_name) == 0)
            return;
        if (!is_batch_drawing_) {
            glEnable(GL_DEPTH_TEST);
            glUseProgram(shader_program_view()->program_id());
            //         auto perspective_matrix =
            //             glm::perspective(glm::radians(110.0f / 2), 1600.0f / 900.0f, 0.1f,
            //             1000.0f);
            //         Point3F look_from = {0.0f, 0.0f, 0.0f};
            //         Point3F look_at = {1.0f, 0.0f, 1.0f};
            //         auto view_matrix = glm::lookAt(
            //             glm::vec3(look_from.x, look_from.y, look_from.z),
            //             glm::vec3(look_at.x, look_at.y, look_at.z), glm::vec3(0.0, 1.0, 0.0));
            //         glUniformMatrix4fv(location_projection_, 1, GL_FALSE,
            //         value_ptr(perspective_matrix)); glUniformMatrix4fv(location_view_, 1,
            //         GL_FALSE, glm::value_ptr(view_matrix));
            XrMatrix4x4f model;
            XrVector3f translation = {0.0f, 0.0f, 0.0f};
            XrQuaternionf rotation = XrQuaternionf();
            XrVector3f scaling = {1.0f, 1.0f, 1.0f};
            XrMatrix4x4f_CreateTranslationRotationScale(&model, &translation, &rotation, &scaling);
            XrMatrix4x4f mvp;
            XrMatrix4x4f_Multiply(&mvp, &viewProjectionMatrix, &model);
            glUniformMatrix4fv(
                location_viewProjection_, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&mvp));
            //         glUniformMatrix4fv(
            //             location_projection_, 1, GL_FALSE,
            //             value_ptr(CameraGl::Get()->GetPerspectiveMatrix()));
            //         auto &view_matrix = CameraGl::Get()->GetViewMatrix();
            //         glUniformMatrix4fv(location_view_, 1, GL_FALSE, glm::value_ptr(view_matrix));
        }
        auto &all_nodes = model_ids_.at(model_name);
        auto p_model = ModelBank::Get()->GetModel(model_name);
        int ms_time_used;
        if (p_model->animDuration == 0)
            ms_time_used = 0;
        else
            ms_time_used =
                static_cast<int>(msTime * global_animation_speed_) % p_model->animDuration;
        auto &model_data = all_nodes;
        for (auto &entry : all_nodes) {
            auto &timeline = entry.second;
            auto found_time = -1.0f;
            const BodyData *p_body_data = nullptr;
            for (auto &keyframe : timeline) {
                auto time = keyframe.first;
                if (ms_time_used >= time)
                    found_time = time;
            }
            auto &body_data = timeline.at(found_time);
            auto image_id = ImageBank::Get()->GetImage(body_data.image_name);
            for (auto i = 0; i < positions.size(); i++) {
                auto position = positions.at(i);
                auto rotation = rotations.at(i);
                auto scaling = scalings.at(i);
                auto colorMod = colorMods.at(i);
                auto brightness = brightnesses.at(i);
                auto model_matrix = glm::rotate(
                    glm::scale(
                        glm::translate(
                            glm::mat4(1.0), glm::vec3(
                                                position.x - Player::Get()->x, position.y,
                                                position.z - Player::Get()->y)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                auto model_no_translation_matrix = glm::rotate(
                    glm::scale(
                        glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0)),
                        glm::vec3(scaling, scaling, scaling)),
                    glm::radians(rotation), glm::vec3(0, 1, 0));
                glUniformMatrix4fv(location_model_, 1, GL_FALSE, glm::value_ptr(model_matrix));
                glUniformMatrix4fv(
                    location_model_no_translation_, 1, GL_FALSE,
                    glm::value_ptr(model_no_translation_matrix));
                glUniform3fv(location_color_mod_, 1, glm::value_ptr(colorMod));
                glm::vec3 viewPos(0.0f, 0.0f, 0.0f);
                //            glm::vec3 viewPos(
                //                Player::Get()->GetSpaceCoord().x,
                //                Player::Get()->GetSpaceCoord().y,
                //                Player::Get()->GetSpaceCoord().z);
                glUniform3fv(location_view_pos_, 1, glm::value_ptr(viewPos));
                glm::vec3 fogColorGl(0.0f, 0.5f, 1.0f);
                glUniform3fv(location_fog_color_, 1, glm::value_ptr(fogColorGl));
                glUniform1f(location_alpha_, brightness);
                glUniform1f(location_no_fog_, 0.0f);
                glUniform1f(location_no_lighting_, 0.0f);
                glBindVertexArray(body_data.rendid);
                glBindTexture(GL_TEXTURE_2D, image_id);
                glDrawElements(GL_TRIANGLES, body_data.num_vertices, GL_UNSIGNED_INT, NULL);
            }
        }
        glBindVertexArray(0);
        if (!is_batch_drawing_)
            glUseProgram(0);
    }

    void RendererModelsView::Cleanup() {
        CleanupBase();
    }
}
