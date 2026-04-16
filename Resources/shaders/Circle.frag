#version 410 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 Pos;

layout(location = 0) out vec4 fragColor;

uniform sampler2D surface;
uniform int BulletType;


void main() {
    float circleDist = length(Pos);
    float cirlceRadius = 1;
    bool inCircle = (circleDist <= circleRadius);

    float diamondRadius = 1;
    float a = max(abx(Pos.x), abx(Pos.y));
    float b = min(abx(Pos.x), abx(Pos.y));
    float diamondDist = 1.55 * a - 0.45 * b;
    bool inDiamond = (diamondDist <= diamondRadius);

    vec4 texColor = texture(surface, uv);

    // 利用 smoothstep 做反鋸齒
    //float alpha = 1.0 - smoothstep(radius - feather, radius, dist);

    if (!inCircle && (!inDiamond || BulletType == 3)) {
        discard; // 剔除兩個形狀之外的空白區域
    }

    // 輸出顏色
    fragColor = texColor;
}