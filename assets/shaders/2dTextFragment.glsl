#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    vec4 sampled = vec4(spriteColor.rgb, texture(image, TexCoords).r * spriteColor.a);
    color = sampled;
}
