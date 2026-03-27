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

class FadeLayer : public Util::GameObject {
private:
    float m_DurationMs = 5000.0f; // 動畫總時長 1.5 秒
    float m_ElapsedTime = 0.0f;   // 已經經過的時間

    float m_CurrentAlpha = 0.0f;

    bool m_IsFinished = false;    // 標記是否已經完全透明
    bool m_reverse = false;

    Util::Color m_Color;


    // 儲存明確型別的指標，方便我們稍後呼叫它獨有的 SetAlpha() 函式
    std::shared_ptr<CustomColorShape> m_FadeShape;

public:
    FadeLayer(Util::Color Color, float DurationMs, bool reverse);

    void Update();

    // 讓遊戲主迴圈判斷是否可以將其從 Renderer 中移除
    bool IsFinished() const { return m_IsFinished; }
};

#endif //JUST_SHAPES_AND_BEATS_FADELAYER_HPP