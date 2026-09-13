#version 300 es
precision mediump float;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    vec4 sampled = vec4(spriteColor.rgb, texture(image, TexCoords).r * spriteColor.a);
    FragColor = sampled;
}
