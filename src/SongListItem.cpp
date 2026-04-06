//
// Created by cheese on 2026/4/6.
//

#include "SongListItem.hpp"

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
