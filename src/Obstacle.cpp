//
// Created by jerry on 2026/3/20.
//

#include "Obstacle.hpp"

void Obstacle::UpdateStateByBeat(float currentBeat) {
    if (currentBeat > m_Event.EndBeat) {
        m_IsDead = true; // 標記為可銷毀
        return;
    }

    //m_Transform.scale = {200, 200};

    // 計算生命週期進度比例 t (0.0 到 1.0 之間)
    float t = (currentBeat - m_Event.StartBeat) / (m_Event.EndBeat - m_Event.StartBeat);
    t = std::clamp(t, 0.0f, 1.0f);

    // 根據節拍進度，利用線性插值 (Lerp) 計算當前的位移與旋轉 [5]
    m_Transform.translation = m_Event.StartPos + (m_Event.EndPos - m_Event.StartPos) * t;
    //m_Transform.rotation = m_Event.StartRot + (m_Event.EndRot - m_Event.StartRot) * t;

    // 若需要隨節拍縮放跳動的特效，可以在此處疊加計算給 m_Transform.scale [5]
    // float beatPulse = 1.0f + 0.2f * sin(currentBeat * glm::pi<float>());
    // m_Transform.scale = glm::vec2(beatPulse);
}

void Obstacle::UpdateWorldVertices() {
    // 每次更新前清空上一幀的資料，並預留足夠的空間以優化效能
    m_WorldVertices.clear();
    m_WorldVertices.reserve(m_LocalVertices.size());

    // 1. 提取 Transform 的數值，並預先算出 sin 與 cos (只算一次，節省效能)
    float scaleX = m_Transform.scale.x;
    float scaleY = m_Transform.scale.y;
    float transX = m_Transform.translation.x;
    float transY = m_Transform.translation.y;

    // 框架的 rotation 單位是弧度(rad)，直接丟入三角函數即可
    float cosTheta = std::cos(m_Transform.rotation);
    float sinTheta = std::sin(m_Transform.rotation);

    // 2. 遍歷 m_LocalVertices 中的每一個頂點 (每 2 個 float 是一組 X, Y)
    for (size_t i = 0; i < m_LocalVertices.size(); i += 2) {
        float localX = m_LocalVertices[i];
        float localY = m_LocalVertices[i + 1];

        // 步驟 A：縮放 (Scale)
        // 先把 1x1 的形狀拉伸成我們設定的寬高
        float scaledX = localX * scaleX;
        float scaledY = localY * scaleY;

        // 步驟 B：旋轉 (Rotate)
        // 套用 2D 旋轉矩陣公式
        float rotatedX = scaledX * cosTheta - scaledY * sinTheta;
        float rotatedY = scaledX * sinTheta + scaledY * cosTheta;

        // 步驟 C：位移 (Translate)
        // 最後加上物件在遊戲世界中的實際位置
        float worldX = rotatedX + transX;
        float worldY = rotatedY + transY;

        // 將算好的絕對座標存入 worldVertices
        m_WorldVertices.push_back(worldX);
        m_WorldVertices.push_back(worldY);
    }
    m_WorldUVs = {0.25f, 0.5f, 0.25f, 0.5f, 0.25f, 0.5f, 0.25f, 0.5f};
}