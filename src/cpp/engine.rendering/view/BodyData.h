#pragma once

namespace nar
{
    struct BodyData {
        RenderId rendid = 0;
        int num_vertices = 0;
        std::string_view image_name= "";
    };
}