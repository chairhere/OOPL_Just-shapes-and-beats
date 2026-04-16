#version 410 core

layout(location = 0) in vec2 vertPosition;
layout(location = 1) in vec2 vertUv;
layout(location = 2) in vec2 LocalPos;

layout(location = 0) out vec2 uv;
layout(location = 1) out vec2 Pos;

// 接收 PTSD 框架傳進來的矩陣資料
layout (std140) uniform Matrices {
    mat4 m_Model;
    mat4 m_Projection;
};

void main() {
    // 乘上矩陣，確保座標與畫面解析度同步
    gl_Position = m_Projection * m_Model * vec4(vertPosition, 0, 1);

    uv = vertUv; // 傳給 Fragment Shader (-1.0 ~ 1.0)
    Pos = LocalPos;
}