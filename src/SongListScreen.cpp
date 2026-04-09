//
// Created by cheese on 2026/3/27.
//

#include "../include/SongListScreen.hpp"

#include "SongList.hpp"

SongListScreen::SongListScreen() {
    //==歌曲清單==
    int listLenth = m_SongsOrder.size();
    float currentX = 0.0f;
    float currentY = 150.0f;

    for (int i = 0 ; i < listLenth ; i++) {
        SongData data = SongList::GetSongByName(m_SongsOrder.at(i));
        std::shared_ptr<SongListItem> item = std::make_shared<SongListItem>(data, currentX, currentY);
        m_Items.push_back(item);
        m_Items.at(i)->SetOnClick([this, i]() {
            m_SelectedIndex = i;
            this->m_NowSelect = this->m_Items.at(m_SelectedIndex);
        });
        m_Items.at(i)->SetOnFocus([this, i]() {
            m_SelectedIndex = i;
            this->m_NowSelect = this->m_Items.at(m_SelectedIndex);
        });
        m_Renderer.AddChild(m_Items.at(i));
        currentY -= m_Items.at(i)->GetSize().y;
    }
    m_SelectedIndex = 0;
    m_Items.at(0)->Focus();

    //==隨機按鈕==
    m_RandomOrder = std::make_shared<Button>("../Resources/Image/OptionBackground/None.png");
    m_RandomOrder->m_Transform.scale = glm::vec2(1.5, 0.1);
    m_RandomOrder->SetFocusImage("../Resources/Image/OptionBackground/Selected.png");
    m_RandomOrder->m_Transform.translation = glm::vec2(0, 250);
    m_RandomOrder->HoverEnable(false);
    m_Renderer.AddChild(m_RandomOrder);
}

ScreenState SongListScreen::Update() {
    int ListLength = m_SongsOrder.size();

    //防Hover與Focus衝突(沒有Hover效果，純控制鼠標顯示)
    if (Util::Input::IsMouseMoving()) {
        Button::s_IsKeyboardMode = false;
        SDL_ShowCursor(SDL_ENABLE);
    }

    // 檢查導航鍵
    if (Util::Input::IsKeyDown(Util::Keycode::W) ||
        Util::Input::IsKeyDown(Util::Keycode::S) ||
        Util::Input::IsKeyDown(Util::Keycode::UP) ||
        Util::Input::IsKeyDown(Util::Keycode::DOWN) ||
        Util::Input::IsKeyDown(Util::Keycode::RETURN)) {

        Button::s_IsKeyboardMode = true;
        SDL_ShowCursor(SDL_DISABLE);

        if (m_NowSelect) {
            m_NowSelect->Unfocus();
            if (Util::Input::IsKeyDown(Util::Keycode::W) ||
                Util::Input::IsKeyDown(Util::Keycode::UP)) {
                if (m_SelectedIndex == 0) {
                    m_SelectedIndex = -1;
                    m_NowSelect = m_RandomOrder;
                }else if (m_SelectedIndex > 0) {
                    m_SelectedIndex -= 1;
                    m_NowSelect = m_Items.at(m_SelectedIndex);
                }
            }else if (Util::Input::IsKeyDown(Util::Keycode::S) ||
                    Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
                if (m_SelectedIndex == -1) {
                    m_SelectedIndex = 0;
                    m_NowSelect = m_Items.at(0);
                }else if (m_SelectedIndex < ListLength-1) {
                    m_SelectedIndex += 1;
                    m_NowSelect = m_Items.at(m_SelectedIndex);
                }
            }
            m_NowSelect->Focus();
        }else {
            LOG_ERROR("觸發空白項目");
            throw std::invalid_argument("The list should NOT be without selected items.");
        }

        if (((m_SelectedIndex == -1) ^ (m_NowSelect == m_RandomOrder)) or (m_SelectedIndex != -1 and m_NowSelect != m_Items.at(m_SelectedIndex))) {
            LOG_WARN("SelectedIndex doesn't match NowSelect");
        }
    }

    m_Renderer.Update();
    m_RandomOrder->Update();
    for (int i = 0 ; i < ListLength ; i++) {
        m_Items.at(i)->Update();
    }

    return ScreenState::LevelList;
}
