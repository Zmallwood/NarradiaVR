/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once
#include "RendererBaseView.h"

namespace nar {
    class RendererFacelockedView : public RendererBaseView,
                                   public Singleton<RendererFacelockedView> {
      public:
        RendererFacelockedView();
        ~RendererFacelockedView();
        RenderId NewRect();
        void SetGeometryRect(
            RenderId vao_id, Vertex3F &v0, Vertex3F &v1, Vertex3F &v2, Vertex3F &v3,
            Point3F &normal00, Point3F &normal10, Point3F &normal11, Point3F &normal01);
        void DrawRect(
            std::string_view image_name, RenderId vao_id, XrMatrix4x4f viewProjectionMatrix,
            bool depth_test_off = false);

      private:
        void DrawImagePolygon(
            std::string_view image_name, RenderId vao_id, int vertex_count,
            XrMatrix4x4f viewProjectionMatrix, bool depth_test_off = false);
        RenderId NewImagePolygon(int num_vertices);
        void SetGeometryImagePolygon(RenderId vao_id, std::vector<Vertex3F> &vertices);

        const int kLocationPosition = 0;
        const int kLocationColor = 1;
        const int kLocationUv = 2;
        const int kLocationNormal = 3;
        int location_projection_ = -1;
        int location_view_ = -1;
        int location_modelViewProjection_ = -1;
        int location_model_ = -1;
        int location_alpha_ = -1;
        int location_view_pos_ = -1;
        int location_fog_color_ = -1;
        bool is_batch_drawing_ = false;
    };
}
