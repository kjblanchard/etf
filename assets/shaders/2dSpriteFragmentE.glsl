#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform vec4 srcRect;

void main()
{
    ivec2 texel = ivec2(srcRect.xy) + ivec2(TexCoords);
    ivec2 maxTexel = ivec2(srcRect.xy + srcRect.zw) - 1;
    texel = clamp(texel, ivec2(srcRect.xy), maxTexel);
    FragColor = spriteColor * texelFetch(image, texel, 0);
}
