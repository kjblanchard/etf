#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;
uniform float time;
uniform vec2 resolution;
void main() {
    const int SAMPLES = 10;
    vec2 uv = TexCoords - vec2(0.5);
    float aspect = resolution.x / resolution.y;
    uv.x *= aspect;

    float dist = length(uv);
    float angle = atan(uv.y, uv.x);

    float swirl = time * 8.0;
    float shrink = 1.0 - time * 0.85;

    float blurArc = time * time * 1.5;
    vec4 accum = vec4(0.0);
    for (int i = 0; i < SAMPLES; ++i) {
        float t = float(i) / float(SAMPLES - 1) - 0.5;
        float sampleAngle = angle + swirl * (1.0 - dist) + blurArc * t;
        vec2 sampleUV;
        sampleUV.x = dist * cos(sampleAngle) / aspect + 0.5;
        sampleUV.y = dist * sin(sampleAngle) + 0.5;
        accum += texture(image, sampleUV);
    }
    accum /= float(SAMPLES);

    float fade = clamp(1.0 - time * 1.4, 0.0, 1.0);
    float centerFade = clamp(dist / (shrink * 0.5 + 0.01), 0.0, 1.0);

    color = spriteColor * vec4(accum.rgb * fade * centerFade, accum.a * fade * centerFade);
}
