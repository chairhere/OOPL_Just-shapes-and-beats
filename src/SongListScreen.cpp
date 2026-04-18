//
// Created by cheese on 2026/3/27.
//

#include "../include/SongListScreen.hpp"

#include "MusicPlayerManager.hpp"
#include "SongList.hpp"

SongListScreen::SongListScreen() {
    //==歌曲清單==
    int listLenth = m_SongsOrder.size();
    float currentX = 0.0f;
    float currentY = 200.0f;

    for (int i = 0 ; i < listLenth ; i++) {
        SongData data = SongList::GetSongByName(m_SongsOrder.at(i).first);
        std::shared_ptr<SongListItem> item = std::make_shared<SongListItem>(data, currentX, currentY);
        m_Items.push_back(item);
        m_Items.at(i)->SetOnClick([this, i]() {
            if (m_SFXSelect != m_Items.at(i)) {
                MusicPlayerManager::Setting().Switch(m_SongsOrder.at(i).first);
                MusicPlayerManager::Setting().PlayAtTime(m_SongsOrder.at(i).second);
                this->m_SFXSelect = m_Items.at(i);
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Choose);
            }else {
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
                play = true;
            }
            this->m_NowSelect->Unfocus();
            this->m_SelectedIndex = i;
            this->m_NowSelect = this->m_Items.at(m_SelectedIndex);
            this->m_NowSelect->Focus();
        });
        m_Items.at(i)->SetOnFocus([this, i]() {
            if (m_SFXSelect != m_Items.at(i)) {
                MusicPlayerManager::Setting().Switch(m_SongsOrder.at(i).first);
                MusicPlayerManager::Setting().PlayAtTime(m_SongsOrder.at(i).second);
                this->m_SFXSelect = m_Items.at(i);
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            }
            this->m_SelectedIndex = i;
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
    m_RandomOrder->SetOnClick([this]() {
        if (m_SFXSelect != m_RandomOrder) {
            MusicPlayerManager::Setting().CleanList();
            this->m_SFXSelect = m_RandomOrder;
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Choose);
        }
        this->m_NowSelect->Unfocus();
        this->m_SelectedIndex = -1;
        this->m_NowSelect = m_RandomOrder;
        this->m_NowSelect->Focus();
    });
    m_RandomOrder->SetOnFocus([this]() {
        if (m_SFXSelect != m_RandomOrder) {
            MusicPlayerManager::Setting().CleanList();
            this->m_SFXSelect = m_RandomOrder;
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
        }
        this->m_SelectedIndex = -1;
        this->m_NowSelect = m_RandomOrder;
    });
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
        Util::Input::IsKeyDown(Util::Keycode::DOWN)) {

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

    if (play || Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
        if (m_NowSelect != m_RandomOrder) {
            return ScreenState::Playground;
        }
        MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrHit);
        Button::s_IsKeyboardMode = true;
        SDL_ShowCursor(SDL_DISABLE);
    }

    m_Renderer.Update();
    m_RandomOrder->Update();
    for (int i = 0 ; i < ListLength ; i++) {
        m_Items.at(i)->Update();
    }

    return ScreenState::LevelList;
}
