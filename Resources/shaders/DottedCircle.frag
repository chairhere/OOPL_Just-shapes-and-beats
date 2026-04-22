#version 410 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec2 Pos;

layout(location = 0) out vec4 fragColor;

uniform sampler2D surface;

void main() {
    float dist = length(Pos);

    // 2. 定義圓環的粗細 (外圈最大是 1.0，內圈決定圓環厚度)
    float outerRadius = 0.5;
    float innerRadius = 0.495;

    // 如果像素不在圓環的厚度範圍內，直接捨棄 (畫出中空圓環)
    if (dist > outerRadius || dist < innerRadius) {
        discard;
    }

    // 3. 計算當前像素的角度 (atan 函數會回傳 -PI 到 PI 的弧度)
    float angle = atan(Pos.y, Pos.x);

    // 4. 設定虛線的段數
    // 注意：這個數字代表會產生多少段「實線」
    float dashCount = 72;

    // 5. 利用 sin 函數產生週期性的波浪，用來切斷圓環
    // sin() 產生的值在大於 0 時繪製，小於 0 時捨棄，藉此形成虛線空白
    if (sin(angle * dashCount) < 0.0) {
        discard;
    }

    // 6. 通過所有考驗的像素，填上顏色

    vec4 textColor = texture(surface, uv);

    fragColor = textColor;
}
