//
// Created by jerry on 2026/3/26.
//
#include "FadeLayer.hpp"

#include "spdlog/fmt/bundled/color.h"

FadeLayer::FadeLayer(Util::Color Color, float DurationMs, bool reverse) {
    m_Color = Color;
    m_DurationMs = DurationMs;
    m_reverse = reverse;
    if (reverse) {
        m_FinishedAlpha = 255.0f;
    }
    else {
        m_FinishedAlpha = 0.0f;
    }
    // 1. 實例化新版的 CustomColorShape，並設定初始為不透明的純黑
    // 利用框架內建的 Util::Color (r, g, b, a)
    m_FadeShape = std::make_shared<CustomColorShape>(m_Color);

    // 2. 將其指定給繼承自 GameObject 的 m_Drawable [1]
    m_Drawable = m_FadeShape;

    // 3. 利用 Transform 將其完美拉伸至全螢幕
    // 新版的形狀能接收 Transform 矩陣，因此這行程式碼會確實把 1x1 紋理放大到整個視窗大小
    m_Transform.scale = glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 4. 確保它在畫面的正中央
    m_Transform.translation = glm::vec2(0.0f, 0.0f);
}

void FadeLayer::Update() {
    if (m_IsFinished) return;

    // 1. 利用框架的計時器獲取這一幀經過的毫秒數並累加 [4]
    m_ElapsedTime += Util::Time::GetDeltaTimeMs();

    // 2. 計算時間進度比例 (介於 0.0 到 1.0 之間)
    float progress = m_ElapsedTime / m_DurationMs;
    m_CurrentAlpha = glm::mix(m_Color.a, m_FinishedAlpha, progress);
    //progress = std::clamp(progress, m_Color.a, m_FinishedAlpha);
    LOG_DEBUG(progress);

    // 3. 計算透明度：progress 越大，Alpha 越接近 0.0f (完全透明)
    // 4. 呼叫新版 CustomColorShape 的專屬函式，高效更新 GPU 裡的紋理透明度
    m_FadeShape->SetAlpha(m_CurrentAlpha);

    // 5. 判斷 1.5 秒是否已經結束
    if (m_ElapsedTime >= m_DurationMs) {
        m_IsFinished = true;
    }
}