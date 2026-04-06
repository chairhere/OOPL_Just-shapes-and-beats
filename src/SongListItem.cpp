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
    m_Background->m_Transform.scale = glm::vec2(50,20);
    m_Children.push_back(m_Background);

    m_Title = std::make_shared<TextObject>(16, data.Title);
    float textWidth = m_Title->GetScaledSize().x;
    float textAnchorX = -300.0f;
    m_Title->m_Transform.translation = glm::vec2(textAnchorX + (textWidth / 2.0f), 0.0f);
    m_Children.push_back(m_Title);

    m_Composer = std::make_shared<TextObject>(16, data.Composer);
    textWidth = m_Composer->GetScaledSize().x;
    textAnchorX = 0.0f;
    m_Composer->m_Transform.translation = glm::vec2(textAnchorX + (textWidth / 2.0f), 0.0f);
    m_Children.push_back(m_Composer);

    m_WhoAmI = data.Level;

}

std::string_view SongListItem::GetName() {
    return to_string(m_WhoAmI);
}

Levels SongListItem::GetLevel() {
    return m_WhoAmI;
}

void SongListItem::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();
    const bool isCurrentlyActive = hovering || focused;

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
            if (!m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}

