/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

R"_(#version 320 es

in vec3 VertexPos;
in vec3 VertexColor;

out vec3 PSVertexColor;

uniform mat4 ModelViewProjection;

void main() {
    gl_Position = ModelViewProjection * vec4(VertexPos, 1.0);
    PSVertexColor = VertexColor;
}
)_"