//
// Created by cheese on 2026/3/20.
//

#include "../include/Button.hpp"

void Button::SetOnClick(const std::function<void()> &onClickEvent) {
    m_OnClick = onClickEvent;
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

