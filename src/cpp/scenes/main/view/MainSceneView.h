/* Copyright (c) 2017-2023, The Khronos Group Inc.

SPDX-License-Identifier: Apache-2.0

This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

#pragma once
#include "../../../engine/view/ISceneView.h"
#include <map>

namespace nar {
    class MainSceneView : public ISceneView {
      public:
        MainSceneView();
        void Render() override;

      private:
        void RenderTile(int map_x, int map_y, XrMatrix4x4f vp);

        std::map<int, std::map<int, RenderId>> rendids_tiles;
        RenderId rendid_facelocked_0_;
        RenderId rendid_facelocked_1_;
        RenderId rendid_facelocked_2_;
        RenderId rendid_facelocked_3_;
    };
}
