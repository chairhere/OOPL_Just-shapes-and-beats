#version 410 core

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D surface;


void main() {
    float dist = length(uv);
    float radius = 0.25;
    float feather = 0.02;

    vec4 texColor = texture(surface, uv);

    // 利用 smoothstep 做反鋸齒
    float alpha = 1.0 - smoothstep(radius - feather, radius, dist);

    if (alpha <= 0.0)
        discard;

    // 輸出顏色
    fragColor = vec4(textColor.rgb, textColor.a * alpha);
}