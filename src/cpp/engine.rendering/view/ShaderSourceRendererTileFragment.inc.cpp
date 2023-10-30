/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

R"_(
    #version 320 es
    precision mediump float;
    vec3 light_direction = vec3(4.5, -3.0, -4.5);
    vec3 light_ambient = vec3(0.8, 0.8, 0.8);
    vec3 light_color = vec3(1.2, 0.7, 0.9);
    in vec4 ex_Color;
    in vec2 ex_TexCoord;
    in vec3 ex_Normal;
    in vec3 FragPos;
    uniform sampler2D TexUnit;
    uniform float mAlpha;
    uniform vec3 viewPos;
    uniform vec3 fogColor;
    out vec4 fragColor;

    void main() {
        vec4 color = vec4(ex_Color.rgb, texture(TexUnit, ex_TexCoord).a);
        if (color.a == 0.0)
            discard;
        vec3 mAmbient = light_ambient * texture(TexUnit, ex_TexCoord).rgb;
        vec3 norm = normalize(ex_Normal);
        vec3 lightDir = normalize(-light_direction);
        float diff = 3.5*max(dot(norm, lightDir), 0.15);
        vec3 mDiffuse = light_color * diff * texture(TexUnit, ex_TexCoord).rgb;
        vec3 result = mAmbient + mDiffuse;
        vec4 result2 = vec4(result.r*color.r, result.g*color.g, result.b*color.b, color.a);
        fragColor = result2;
    }
)_"