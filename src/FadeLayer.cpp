//
// Created by jerry on 2026/3/26.
//
#include "FadeLayer.hpp"

#include "spdlog/fmt/bundled/color.h"

FadeLayer::FadeLayer(const std::vector<Util::Color> &Colors, const std::vector<float> &Durations,
    const std::vector<glm::vec2> &Positions, const std::vector<float> &Rotation,
    const std::vector<glm::vec2> &Scales, const std::vector<float> &Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f}, const bool loop = false) {

    m_Colors = Colors;
    m_Durations = Durations;
    m_Positions = Positions;
    m_Scales = Scales;
    m_Loop = loop;
    m_Rotations = Rotation;
    m_LocalVertices = Vertices;

    m_LoopSize = static_cast<int>(m_Durations.size());

    m_Color = Colors[0];
    m_FinishedColor = Colors[1];

    m_DurationMs = Durations[1] - Durations[0];

    m_Position = Positions[0];
    m_FinishedPosition = Positions[1];

    m_Scale = Scales[0];
    m_FinishedScale = Scales[1];

    m_Rotation = Rotation[0];
    m_FinishedRotation = Rotation[1];

    ImGui::ColorConvertRGBtoHSV(m_Color.r, m_Color.g, m_Color.b, m_H, m_S, m_V);
    ImGui::ColorConvertRGBtoHSV(m_FinishedColor.r, m_FinishedColor.g, m_FinishedColor.b, m_FinishedH, m_FinishedS, m_FinishedV);


    // 1. 實例化新版的 CustomColorShape，並設定初始為不透明的純黑
    // 利用框架內建的 Util::Color (r, g, b, a)
    m_FadeShape = std::make_shared<CustomColorShape>(m_CurrentColor, m_LocalVertices);

    // 2. 將其指定給繼承自 GameObject 的 m_Drawable [1]
    m_Drawable = m_FadeShape;

    // 3. 利用 Transform 將其完美拉伸至全螢幕
    // 新版的形狀能接收 Transform 矩陣，因此這行程式碼會確實把 1x1 紋理放大到整個視窗大小
    m_Transform.scale = m_Scale;

    // 4. 確保它在畫面的正中央
    m_Transform.translation = m_Position;

    m_Transform.rotation = m_Rotation;


}

void FadeLayer::Update() {
    if (m_IsFinished) return;

    // 1. 利用框架的計時器獲取這一幀經過的毫秒數並累加 [4]
    m_ElapsedTime += Util::Time::GetDeltaTimeMs();
    if (m_ElapsedTime >= m_DurationMs) {
        m_ElapsedTime = m_DurationMs;
    }
    // 2. 計算時間進度比例 (介於 0.0 到 1.0 之間)
    float progress = m_ElapsedTime / m_DurationMs;

    m_CurrentH = glm::mix(m_H, m_FinishedH, progress);
    m_CurrentS = glm::mix(m_S, m_FinishedS, progress);
    m_CurrentV = glm::mix(m_V, m_FinishedV, progress);

    m_CurrentColor.a = glm::mix(m_Color.a, m_FinishedColor.a, progress);

    m_CurrentPosition = glm::mix(m_Position, m_FinishedPosition, progress);
    m_CurrentScale = glm::mix(m_Scale, m_FinishedScale, progress);
    m_CurrentRotation = glm::mix(m_Rotation, m_FinishedRotation, progress);
/*
    ImGui::Begin("test");
    ImGui::SetWindowPos({200, 300});
    ImGui::Text("%d", m_Counter);
    ImGui::End();
*/
    //ImGui::ShowDemoWindow();

    ImGui::ColorConvertHSVtoRGB(m_CurrentH, m_CurrentS, m_CurrentV, m_CurrentColor.r, m_CurrentColor.g, m_CurrentColor.b);
    //LOG_DEBUG(progress);

    // 3. 計算透明度：progress 越大，Alpha 越接近 0.0f (完全透明)
    // 4. 呼叫新版 CustomColorShape 的專屬函式，高效更新 GPU 裡的紋理透明度
    m_FadeShape->SetColors(m_CurrentColor);

    m_Transform.scale = m_CurrentScale;

    m_Transform.translation = m_CurrentPosition;

    m_Transform.rotation = m_CurrentRotation;


    // 5. 判斷整體是否已經結束
    if (m_ElapsedTime >= m_DurationMs && m_Counter < (m_LoopSize - 2)) {
        m_Counter++;
        State_Update();
    }
    else if (m_ElapsedTime >= m_DurationMs && m_Counter >= (m_LoopSize - 2) && m_Loop) {
        m_Counter = 0;
        State_Update();
    }
    else if (m_ElapsedTime >= m_DurationMs){
        m_IsFinished = true;
    }
}

void FadeLayer::State_Update() {
    m_Color = m_Colors[m_Counter];
    m_FinishedColor = m_Colors[m_Counter + 1];

    m_DurationMs = m_Durations[m_Counter + 1] - m_Durations[m_Counter];
    m_ElapsedTime = 0.0f;

    m_Scale = m_Scales[m_Counter];
    m_FinishedScale = m_Scales[m_Counter + 1];

    m_Position = m_Positions[m_Counter];
    m_FinishedPosition = m_Positions[m_Counter + 1];

    m_Rotation = m_Rotations[m_Counter];
    m_FinishedRotation = m_Rotations[m_Counter + 1];
}
