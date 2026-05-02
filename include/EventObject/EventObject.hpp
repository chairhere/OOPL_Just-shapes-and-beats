//
// Created by cheese on 2026/4/6.
//

#ifndef JUST_SHAPES_AND_BEATS_EVENTOBJECT_HPP
#define JUST_SHAPES_AND_BEATS_EVENTOBJECT_HPP
#include <functional>

#include "Util/GameObject.hpp"

class EventObject : public Util::GameObject {
public:
    // 全域共用的鍵盤模式標記，用來防止 Hover 雙重觸發
    static bool s_IsKeyboardMode;

    void SetOnClick(const std::function<void()> &onClickEvent);
    void SetOnHovering(const std::function<void()> &onHoverEvent);
    void SetOnFocus(const std::function<void()> &onFocusEvent);
    void SetOffEvent(const std::function<void()> &offEvent);

    void HoverEnable(const bool enable);
    virtual bool isHovering() const;

    void FocusEnable(const bool enable);
    void Focus();
    void Unfocus();
    bool isFocus() const;

    bool isActive() const;

    virtual void Update() = 0;

protected:
    bool m_HoverEnable = true;
    bool m_FocusEnable = true;
    bool m_Focus = false;
    std::function<void()> m_OnClick = nullptr;
    std::function<void()> m_OnHover = nullptr;
    std::function<void()> m_OnFocus = nullptr;
    std::function<void()> m_OffEvent = nullptr;

    // --- 自動還原系統 (Snapshot) 變數 ---
    bool m_WasActive = false;
    Util::Transform m_NormalTransform;   // 備份位移、縮放、旋轉
};

#endif //JUST_SHAPES_AND_BEATS_EVENTOBJECT_HPP