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
    //  不允許 Hover
    if (!m_HoverEnable) return false;

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

/* Update 基礎格式
void Template::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();
    const bool isCurrentlyActive = hovering || focused;

    if (isCurrentlyActive) {
        // 【剛碰到的第一幀】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;

            //!備份

            m_WasActive = true;
        }

        // 觸發自定義的特效或點擊事件
        if (hovering) {
            if (m_OnHover) m_OnHover();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                if (m_OnClick) m_OnClick();
            }
        } else if (focused) {
            if (m_OnFocus) m_OnFocus();
            if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
                if (m_OnClick) m_OnClick();
            }
        }
    } else {
        // 【離開的第一幀】還原所有狀態
        if (m_WasActive) {
            m_Transform = m_NormalTransform;

            //!還原

            if (!m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}
*/