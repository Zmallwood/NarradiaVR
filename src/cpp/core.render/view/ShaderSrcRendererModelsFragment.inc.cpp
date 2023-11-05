/* (c) 2023 Zmallwood
This code is licensed under Apache License, Version 2.0 (see LICENSE for details) */

R"_(
    #version 320 es
    precision mediump float;
    vec3 light_direction = -vec3(0.0, -0.5, 4.5);
    vec3 light_ambient = vec3(0.65, 0.65, 0.65);
    vec3 light_color = vec3(0.6, 0.6, 0.5);
    in vec4 ex_Color;
    in vec2 ex_TexCoord;
    in vec3 ex_Normal;
    in float FogCoord;
    in vec3 FragPos;
    uniform sampler2D TexUnit;
    uniform float mAlpha;
    uniform vec3 mColorMod;
    uniform vec3 viewPos;
    uniform vec3 fogColor;
    uniform float noFog;
    uniform float noLighting;
    out vec4 fragColor;
    float nearPlane = 0.1;
    float farPlane = 65.0;
    void main() {
        vec4 color = vec4(ex_Color.rgb, texture(TexUnit, ex_TexCoord).a);
        if (color.a == 0.0)
            discard;
        vec3 result;
        vec3 mAmbient = light_ambient * texture(TexUnit, ex_TexCoord).rgb;
        if (noLighting == 0.0)
        {
            vec3 norm = normalize(ex_Normal);
            vec3 lightDir =normalize(-light_direction);
            float diff =  1.5*max(dot(norm, lightDir), 0.15);
            vec3 mDiffuse = light_color * diff * texture(TexUnit, ex_TexCoord).rgb;
            result = mAmbient + mDiffuse;
        }
        else
        {
            result = mAmbient;
        }
        fragColor = vec4(result*mAlpha, texture(TexUnit, ex_TexCoord).a);
        fragColor.r = fragColor.r*mColorMod.r;
        fragColor.g = fragColor.g*mColorMod.g;
        fragColor.b = fragColor.b*mColorMod.b;
    }
)_"
