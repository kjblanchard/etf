#version 300 es

precision mediump float;

in vec2 TexCoords;

layout(location = 0) out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;

void main()
{
    const int SAMPLES = 8;
    vec2 uv = TexCoords;
    float lineHeight = 2.0;
    float row = floor((uv.y * resolution.y) / lineHeight);
    float lineNoise = fract(sin(row * 43.758) * 2137.193);
    float lineSpeed = 0.4 + lineNoise * 1.2;
    float pullStrength = (time * 0.3 + time * time * 1.8) * lineSpeed;
    uv.x -= pullStrength;

    float blurAmount = (time * 0.3 + time * time * 0.5) * lineSpeed;

    vec4 accum = vec4(0.0);

    for (int i = 0; i < SAMPLES; ++i)
    {
        float t = float(i) / float(SAMPLES - 1);
        vec2 offset = vec2(blurAmount * t, 0.0);
        accum += texture(image, uv + offset);
    }

    accum /= float(SAMPLES);

    float fade = clamp(1.0 - time * 0.5, 0.0, 1.0);

    color = spriteColor * vec4(accum.rgb * fade, accum.a * fade);
}
