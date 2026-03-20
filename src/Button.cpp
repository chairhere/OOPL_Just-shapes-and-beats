//
// Created by cheese on 2026/3/20.
//

#include "../include/Button.hpp"

void Button::SetOnClick(const std::function<void()> &onClickEvent) {
    m_OnClick = onClickEvent;
}

void Button::SetOnHovering(const std::function<void()> &onHoverEvent) {
    m_OnHover = onHoverEvent;
}

void Button::SetOnFocus(const std::function<void()> &onFocusEvent) {
    m_OnFocus = onFocusEvent;
}

bool Button::isHovering() {
    // 1. 取得滑鼠當前位置
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    // 2. 計算按鈕的實際寬高 (考量 Transform 的縮放)
    glm::vec2 size = m_Drawable->GetSize() * m_Transform.scale;
    glm::vec2 pos = m_Transform.translation;

    // 3. 簡單的碰撞偵測 AABB (假設 translation 座標代表按鈕中心)
    bool hovering = (mousePos.x >= pos.x - size.x / 2.0f &&
                       mousePos.x <= pos.x + size.x / 2.0f &&
                       mousePos.y >= pos.y - size.y / 2.0f &&
                       mousePos.y <= pos.y + size.y / 2.0f);

    return hovering;
}

void Button::Update() {
    if (isHovering()) {
        m_OnHover();
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            m_OnClick();
        }
    }
    if (isFocus()) {
        m_OnFocus();
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            m_OnClick();
        }
    }
}
