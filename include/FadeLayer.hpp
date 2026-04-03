//
// Created by jerry on 2026/3/26.
//

#ifndef JUST_SHAPES_AND_BEATS_FADELAYER_HPP
#define JUST_SHAPES_AND_BEATS_FADELAYER_HPP

#include "Util/GameObject.hpp"
#include "Util/Time.hpp"
#include "config.hpp" // 引入 WINDOW_WIDTH 與 WINDOW_HEIGHT
#include "CustomColorShape.hpp" // 假設您的 CustomColorShape 存在此標頭檔中
#include <algorithm>
#include <memory>
#include "imgui.h"

class FadeLayer : public Util::GameObject {
private:
    std::vector<float> m_Durations;
    std::vector<float> m_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    std::vector<float> m_Rotations;
    std::vector<Util::Color> m_Colors;
    std::vector<glm::vec2> m_Positions;
    std::vector<glm::vec2> m_Scales;

    float m_DurationMs = 5000.0f;
    float m_ElapsedTime = 0.0f;

    float m_H = 0.0f, m_S = 0.0f, m_V = 0.0f;
    float m_CurrentH = 0.0f, m_CurrentS = 0.0f, m_CurrentV = 0.0f;
    float m_FinishedH = 0.0f, m_FinishedS = 0.0f, m_FinishedV = 0.0f;

    float m_Rotation = 0.0f, m_CurrentRotation = 0.0f, m_FinishedRotation = 0.0f;

    bool m_IsFinished = false;    // 標記是否已經完全透明
    bool m_Loop = false;

    int m_Counter = 0;
    int m_LoopSize = 0;

    Util::Color m_Color{}, m_CurrentColor{}, m_FinishedColor{};

    glm::vec2 m_Position{}, m_CurrentPosition{}, m_FinishedPosition{};
    glm::vec2 m_Scale{}, m_CurrentScale{}, m_FinishedScale{};


    // 儲存明確型別的指標，方便我們稍後呼叫它獨有的 SetAlpha() 函式
    std::shared_ptr<CustomColorShape> m_FadeShape;

public:
    FadeLayer(const std::vector<Util::Color> &Colors, const std::vector<float> &Durations,
        const std::vector<glm::vec2> &Positions, const std::vector<float> &Rotation,
        const std::vector<glm::vec2> &Scales, const std::vector<float> &Vertices, bool loop);

    void Update();

    void State_Update();


    // 讓遊戲主迴圈判斷是否可以將其從 Renderer 中移除
    bool IsFinished() const { return m_IsFinished; }
};

#endif //JUST_SHAPES_AND_BEATS_FADELAYER_HPP