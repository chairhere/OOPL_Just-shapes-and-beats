//
// Created by cheese on 2026/4/6.
//

#include "../include/EventObject.hpp"

#include "Util/Input.hpp"

bool EventObject::s_IsKeyboardMode = false;

void EventObject::SetOnClick(const std::function<void()> &onClickEvent) {
    m_OnClick = onClickEvent;
}

void EventObject::SetOnHovering(const std::function<void()> &onHoverEvent) {
    m_OnHover = onHoverEvent;
}

void EventObject::SetOnFocus(const std::function<void()> &onFocusEvent) {
    m_OnFocus = onFocusEvent;
}

void EventObject::SetOffEvent(const std::function<void()> &offEvent) {
    m_OffEvent = offEvent;
}

void EventObject::HoverEnable(const bool enable) {
    m_HoverEnable = enable;
}

bool EventObject::isHovering() const {
    if (m_Drawable == nullptr) return false;

    // 【防護】如果是鍵盤模式，一律無視滑鼠 Hover
    if (s_IsKeyboardMode) return false;

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    glm::vec2 size = m_Drawable->GetSize() * m_Transform.scale;
    glm::vec2 pos = m_Transform.translation;

    return (mousePos.x >= pos.x - size.x / 2.0f &&
            mousePos.x <= pos.x + size.x / 2.0f &&
            mousePos.y >= pos.y - size.y / 2.0f &&
            mousePos.y <= pos.y + size.y / 2.0f);
}

void EventObject::FocusEnable(const bool enable) {
    m_FocusEnable = enable;
}

void EventObject::Focus() {
    m_Focus = true;
}

void EventObject::Unfocus() {
    m_Focus = false;
}

bool EventObject::isFocus() const {
    return m_Focus;
}

bool EventObject::isActive() const {
    return (m_HoverEnable && isHovering()) || (m_FocusEnable && m_Focus);
}


/* Update 基礎格式
void Template::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();

    // ==========================================
    // 1. 處理互動邏輯 (獨立於視覺狀態之外！)
    // ==========================================
    if (hovering) {
        if (m_OnHover) m_OnHover();
        // 只要滑鼠在上面，不管 HoverEnable 是不是 false，都可以點擊！
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            if (m_OnClick) m_OnClick();
        }
    } else if (focused) {
        if (m_OnFocus) m_OnFocus();
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            if (m_OnClick) m_OnClick();
        }
    }

    // ==========================================
    // 2. 處理視覺狀態 (這裡才套用 HoverEnable / FocusEnable 的限制)
    // ==========================================
    // 重新定義：這個物件「視覺上」是否該呈現亮起狀態？
    const bool shouldShowVisualActive = (m_HoverEnable && hovering) || (m_FocusEnable && focused);

    if (shouldShowVisualActive) {
        // 【該亮起時】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;
            m_Background->SetImage(m_FocusBackground);
            m_WasActive = true;
        }
    } else {
        // 【該熄滅時】還原所有狀態
        if (m_WasActive) {
            m_Transform = m_NormalTransform;
            m_Background->SetImage(m_NormalBackground);
            if (m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}
*/