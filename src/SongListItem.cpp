//
// Created by cheese on 2026/4/6.
//

#include "SongListItem.hpp"

#include "Util/Input.hpp"

SongListItem::SongListItem(SongData data, float startX, float startY) {
    // 設定這整個 ListItem (父物件) 在畫面上的絕對位置
    m_Transform.translation = glm::vec2(startX, startY);

    m_Background = std::make_shared<ImageObject>(m_NormalBackground);
    // 背景相對於父物件置中
    m_Background->m_Transform.translation = glm::vec2(0, 0);
    m_Background->m_Transform.scale = glm::vec2(1.5,0.1);
    m_Background->SetZIndex(0);
    m_Children.push_back(m_Background);

    m_Checker = std::make_shared<ImageObject>("../Resources/Image/Obstacles/Obstacle_Circle.png");
    float ImageWidth = m_Checker->GetScaledSize().x;
    float ImageAnchorX = -300.0f;
    m_Checker->m_Transform.translation = glm::vec2(ImageAnchorX - (ImageWidth / 2.0f), 0.0f);
    m_Checker->m_Transform.scale = glm::vec2(0.2, 0.2);
    m_Checker->SetZIndex(1);
    m_Children.push_back(m_Checker);

    m_Title = std::make_shared<TextObject>(26, data.Title);
    float textWidth = m_Title->GetScaledSize().x;
    float textAnchorX = -300.0f;
    m_Title->m_Transform.translation = glm::vec2(textAnchorX + (textWidth / 2.0f), 0.0f);
    m_Title->SetZIndex(1);
    m_Children.push_back(m_Title);

    m_Composer = std::make_shared<TextObject>(26, data.Composer);
    textWidth = m_Composer->GetScaledSize().x;
    textAnchorX = 0.0f;
    m_Composer->m_Transform.translation = glm::vec2(textAnchorX + (textWidth / 2.0f), 0.0f);
    m_Composer->SetZIndex(1);
    m_Children.push_back(m_Composer);

    m_WhoAmI = data.Level;
    HoverEnable(false);

}

std::string_view SongListItem::GetName() {
    return to_string(m_WhoAmI);
}

Levels SongListItem::GetLevel() {
    return m_WhoAmI;
}

bool SongListItem::isHovering() const {
    if (m_Drawable == nullptr) return false;

    // 【防護】如果是鍵盤模式，一律無視滑鼠 Hover
    if (s_IsKeyboardMode) return false;

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    glm::vec2 size = m_Background->GetScaledSize();
    glm::vec2 pos = m_Transform.translation;

    return (mousePos.x >= pos.x - size.x / 2.0f &&
            mousePos.x <= pos.x + size.x / 2.0f &&
            mousePos.y >= pos.y - size.y / 2.0f &&
            mousePos.y <= pos.y + size.y / 2.0f);
}


glm::vec2 SongListItem::GetSize() {
    return m_Background->GetScaledSize();
}


void SongListItem::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();
    const bool isCurrentlyActive = isActive();

    if (isCurrentlyActive) {
        // 【剛碰到的第一幀】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;
            m_Background->SetImage(m_FocusBackground);
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
            m_Background->SetImage(m_NormalBackground);
            if (m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}

