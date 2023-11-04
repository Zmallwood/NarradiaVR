/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "../../common/system/Typedefs.h"
#include "BodyData.h"
#include "RendererBaseView.h"
#include <glm/glm.hpp>
#include <map>
#include <string_view>
#include <vector>

namespace nar {
    class RendererModelsView : public RendererBaseView, public Ston<RendererModelsView> {
      public:
        RendererModelsView();
        void NewModel(std::string_view model_name);
        void DrawModel(
            std::string_view model_name, float ms_time, Point3F position,
            XrMatrix4x4f view_projection_matrix, float rotation = 0.0f, float scaling = 1.0f,
            float brightness = 1.0f, glm::vec3 color_mod = {1.0f, 1.0f, 1.0f}, bool no_fog = false,
            bool no_lighting = false, bool cull_face_back = false) const;
        void DrawModelsMany(
            std::string_view model_name, float, std::vector<Point3F>, std::vector<float>,
            std::vector<float>, std::vector<float>, std::vector<glm::vec3>,
            XrMatrix4x4f viewProjectionMatrix) const;
        void StartBatchDrawing(XrMatrix4x4f viewProjectionMatrix);
        void StopBatchDrawing();
        void Cleanup();

      private:
        RenderId NewBodyKeyframe(std::string_view model_name, float, int);
        void NewBodyKeyframeGeometry(
            std::string_view model_name, float, RenderId, std::vector<Vertex3F>,
            std::vector<Point3F>);

        const int kLocationPosition = 0;
        const int kLocationColor = 1;
        const int kLocationUv = 2;
        const int kLocationNormal = 3;
        int location_projection_ = -1;
        int location_view_ = -1;
        int location_alpha_ = -1;
        int location_model_ = -1;
        int location_model_no_translation_ = -1;
        int location_viewProjection_ = -1;
        int location_color_mod_ = -1;
        int location_view_pos_ = -1;
        int location_fog_color_ = -1;
        int location_no_fog_ = -1;
        int location_no_lighting_ = -1;
        std::map<std::string_view, std::map<int, std::map<float, const BodyData>>> model_ids_;
        std::map<std::string_view, std::map<float, RenderId>> timelines_;
        float global_animation_speed_ = 1.0f;
        bool is_batch_drawing_ = false;
    };
}
