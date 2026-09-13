#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;
uniform float time;
uniform float seed;

void main()
{
    // Sink left and down over time
    vec2 drift = vec2(-1.0, 1.0) * time * 6.0;
    vec2 samplePos = TexCoords - drift;
    if (samplePos.x < 0.0 || samplePos.x >= srcRect.z || samplePos.y < 0.0 || samplePos.y >= srcRect.w)
        discard;
    ivec2 texel = ivec2(srcRect.xy) + ivec2(samplePos);
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    vec4 sampled = texelFetch(image, texel, 0);
    if (sampled.a < 0.01)
        discard;
    // Slight red tint that fades out naturally
    vec3 redTint = vec3(1.0, 0.25, 0.2);
    /* vec3 tinted = mix(sampled.rgb, redTint, 0.35); */
    vec3 tinted = mix(sampled.rgb, redTint, 0.45);
    // Fade away
    float alpha = sampled.a * (1.0 - smoothstep(0.0, 1.0, time));
    color = spriteColor * vec4(tinted, alpha);
}
