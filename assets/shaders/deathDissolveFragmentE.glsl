#version 300 es

precision mediump float;
precision highp int;

in vec2 TexCoords;
out vec4 color;

uniform highp sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;
uniform float seed;

void main()
{
    // Sink left and down over time
    vec2 drift = vec2(-6.0, 6.0) * time;
    vec2 samplePos = TexCoords - drift;
    // Outside sprite bounds
    if (samplePos.x < 0.0 || samplePos.y < 0.0 || samplePos.x >= srcRect.z || samplePos.y >= srcRect.w)
    {
        discard;
    }

    ivec2 base = ivec2(srcRect.xy);
    ivec2 size = ivec2(srcRect.zw);

    ivec2 texel = base + ivec2(samplePos);
    texel = clamp(texel, base, base + size - ivec2(1));

    vec4 sampled = texelFetch(image, texel, 0);

    if (sampled.a < 0.01)
        discard;
    // Red tint
    const vec3 redTint = vec3(1.0, 0.25, 0.2);
    vec3 tinted = mix(sampled.rgb, redTint, 0.45);

    // Fade out
    float alpha = sampled.a * (1.0 - smoothstep(0.0, 1.0, time));

    color = spriteColor * vec4(tinted, alpha);
}
