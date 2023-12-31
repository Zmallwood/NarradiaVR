/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

R"_(
    #version 320 es
    layout (location = 0) in vec3 in_Position;
    layout (location = 1) in vec4 in_Color;
    layout (location = 2) in vec2 in_TexCoord;
    layout (location = 3) in vec3  in_Normal;
    out vec4 ex_Color;
    out vec2 ex_TexCoord;
    out vec3 ex_Normal;
    out vec3 FragPos;
    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;
    uniform mat4 modelNoTranslation;
    uniform mat4 viewProjection;
    void main() {
        vec4 newVertex;
        vec4 newNormal;
        newVertex.xyz = in_Position;
        newVertex.w = 1.0;
        newNormal.xyz = in_Normal;
        newNormal.w = 1.0;
        ex_Color = in_Color;
//        gl_Position = projection * view * model * newVertex;
        gl_Position = viewProjection * model * newVertex;
        ex_TexCoord = in_TexCoord;
        ex_Normal = (modelNoTranslation * newNormal).xyz;
        FragPos = vec3(model * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
    }
)_"
