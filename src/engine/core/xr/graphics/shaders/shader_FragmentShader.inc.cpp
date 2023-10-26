/* Copyright (c) 2017-2023, The Khronos Group Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * THIS FILE WAS MODIFIED FROM ITS ORIGINAL VERSION BY Zmallwood FOR Narradia. THE ORIGINAL
 * LICENSE IS STATED IN LICENSE FILE.
 */

R"_(#version 320 es

in lowp vec3 PSVertexColor;
out lowp vec4 FragColor;

void main() {
    FragColor = vec4(PSVertexColor, 1);
}
)_"