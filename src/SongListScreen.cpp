//
// Created by cheese on 2026/3/27.
//

#include "../include/SongListScreen.hpp"

#include "SongList.hpp"

SongListScreen::SongListScreen() {
    int listLenth = m_SongsOrder.size();
    float currentX = 0.0f;
    float currentY = 50.0f;

    for (int i = 0 ; i < listLenth ; i++) {
        SongData data = SongList::GetSongByName(m_SongsOrder.at(i));
        std::shared_ptr<SongListItem> item = std::make_shared<SongListItem>(data, currentX, currentY);
        m_Items.push_back(item);
        m_Items.at(i)->SetOnClick([this, i]() {
            m_SelectedIndex = i;
            this->m_NowSelect = this->m_Items.at(m_SelectedIndex);
        });
        m_Renderer.AddChild(m_Items.at(i));
        currentY -= item->GetScaledSize().y;
    }
    m_SelectedIndex = 0;
    m_Items.at(0)->Focus();
}

ScreenState SongListScreen::Update() {
    return ScreenState::LevelList;
}
