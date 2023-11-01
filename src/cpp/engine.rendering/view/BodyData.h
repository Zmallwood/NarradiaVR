/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

#pragma once

namespace nar
{
    struct BodyData {
        RenderId rendid = 0;
        int num_vertices = 0;
        std::string_view image_name= "";
    };
}