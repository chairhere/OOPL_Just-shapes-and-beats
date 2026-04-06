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

void EventObject::SetOffEvent(const std::function<void()> &OffEvent) {
    m_OffEvent = OffEvent;
}

bool EventObject::isHovering() {
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
