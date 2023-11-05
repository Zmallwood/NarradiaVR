/* Copyright (c) 2017-2023, The Khronos Group Inc.
 
SPDX-License-Identifier: Apache-2.0 
 
This file was modified from its original version by Zmallwood for Narradia.
The original icense is stated in the LICENSE file. */

R"_(#version 320 es

in lowp vec3 PSVertexColor;
out lowp vec4 FragColor;

void main() {
    FragColor = vec4(PSVertexColor, 1);
}
)_"
