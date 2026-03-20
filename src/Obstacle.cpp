//
// Created by jerry on 2026/3/20.
//

#include "Obstacle.hpp"

void Obstacle::UpdateStateByBeat(float currentBeat) {
    if (currentBeat > m_Event.endBeat) {
        m_IsDead = true; // 標記為可銷毀
        return;
    }

    // 計算生命週期進度比例 t (0.0 到 1.0 之間)
    float t = (currentBeat - m_Event.startBeat) / (m_Event.endBeat - m_Event.startBeat);
    t = std::clamp(t, 0.0f, 1.0f);

    // 根據節拍進度，利用線性插值 (Lerp) 計算當前的位移與旋轉 [5]
    m_Transform.translation = m_Event.startPos + (m_Event.endPos - m_Event.startPos) * t; [1]
    m_Transform.rotation = m_Event.startRot + (m_Event.endRot - m_Event.startRot) * t;    [1]

    // 若需要隨節拍縮放跳動的特效，可以在此處疊加計算給 m_Transform.scale [5]
    // float beatPulse = 1.0f + 0.2f * sin(currentBeat * glm::pi<float>());
    // m_Transform.scale = glm::vec2(beatPulse);
};