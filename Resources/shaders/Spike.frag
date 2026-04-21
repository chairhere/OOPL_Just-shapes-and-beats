#version 410 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 Pos;

layout(location = 0) out vec4 fragColor;

uniform sampler2D surface;
uniform int BulletType;


void main() {
    float circleDist = length(Pos);
    float circleRadius = 0.5;
    float feather = 0.02;
    bool inCircle = (circleDist <= circleRadius);

    float diamondRadius = 0.5;
    float a = max(abs(Pos.x), abs(Pos.y));
    float b = min(abs(Pos.x), abs(Pos.y));
    float diamondDist = 1.55 * a - 0.45 * b;
    bool inDiamond = (diamondDist <= diamondRadius);

    // 利用 smoothstep 做反鋸齒
    if (!inCircle && !inDiamond){
        discard;
    }

    vec4 textColor = texture(surface, uv);
    fragColor = textColor;
    //float alpha = 1.0 - smoothstep(radius - feather, radius, dist);
    // 輸出顏色
}