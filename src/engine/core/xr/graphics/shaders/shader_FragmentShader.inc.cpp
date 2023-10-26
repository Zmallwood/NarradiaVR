R"_(#version 320 es

in lowp vec3 PSVertexColor;
out lowp vec4 FragColor;

void main() {
    FragColor = vec4(PSVertexColor, 1);
}
)_"