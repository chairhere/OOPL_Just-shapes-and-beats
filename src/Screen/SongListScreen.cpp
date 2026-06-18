//
// Created by cheese on 2026/3/27.
//

#include "Screen/SongListScreen.hpp"

#include "Manager/MusicPlayerManager.hpp"
#include "Tool/SongList.hpp"

SongListScreen::SongListScreen() {
    //==歌曲清單==
    int listLenth = m_SongsOrder.size();
    float currentX = 0.0f;
    float currentY = 200.0f;

    for (int i = 0 ; i < listLenth ; i++) {
        SongData data = SongList::GetSongByName(m_SongsOrder.at(i));
        std::shared_ptr<SongListItem> item = std::make_shared<SongListItem>(data, currentX, currentY);
        m_Items.push_back(item);
        item->SetOnClick([this, item, i]() {
            this->m_NowSelect->Unfocus();
            this->m_SelectedIndex = i;
            this->m_NowSelect = item;
            this->m_NowSelect->Focus();
            if (m_SFXSelect != item) {
                MusicPlayerManager::Setting().Switch(m_SongsOrder.at(i));
                this->m_SFXSelect = item;
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Choose);
            }else {
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
                play = true;
            }
        });
        m_Items.at(i)->SetOnFocus([this, item, i]() {
            if (m_SFXSelect != item) {
                MusicPlayerManager::Setting().Switch(m_SongsOrder.at(i));
                this->m_SFXSelect = item;
                MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            }
        });
        m_Renderer.AddChild(item);
        currentY -= item->GetSize().y;
    }
    m_SelectedIndex = 0;
    m_NowSelect = m_Items.at(0);
    m_Items.at(0)->Focus();

    //==隨機按鈕==目前尚無此功能
    // m_RandomOrder = std::make_shared<Button>("../Resources/Image/OptionBackground/None.png");
    // m_RandomOrder->m_Transform.scale = glm::vec2(1.5, 0.1);
    // m_RandomOrder->SetFocusImage("../Resources/Image/OptionBackground/Selected.png");
    // m_RandomOrder->m_Transform.translation = glm::vec2(0, 250);
    // m_RandomOrder->SetOnClick([this]() {
    //     if (m_SFXSelect != m_RandomOrder) {
    //         MusicPlayerManager::Setting().CleanList();
    //         this->m_SFXSelect = m_RandomOrder;
    //         MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::Choose);
    //     }
    //     this->m_NowSelect->Unfocus();
    //     this->m_SelectedIndex = -1;
    //     this->m_NowSelect = m_RandomOrder;
    //     this->m_NowSelect->Focus();
    // });
    // m_RandomOrder->SetOnFocus([this]() {
    //     if (m_SFXSelect != m_RandomOrder) {
    //         MusicPlayerManager::Setting().CleanList();
    //         this->m_SFXSelect = m_RandomOrder;
    //         MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
    //     }
    //     this->m_SelectedIndex = -1;
    //     this->m_NowSelect = m_RandomOrder;
    // });
    // m_RandomOrder->HoverEnable(false);
    // m_Renderer.AddChild(m_RandomOrder);
    MusicPlayerManager::Setting().Switch(m_SongsOrder.at(0));

    m_Hint = std::make_shared<ImageObject>("../Resources/Image/MainScreenButton/Hint.png");
    m_Hint->m_Transform.translation = glm::vec2(400, -370);
    m_Renderer.AddChild(m_Hint);
}

ScreenState SongListScreen::Update() {
    int ListLength = m_SongsOrder.size();

    if (not freeze) {
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
                    /*if (m_SelectedIndex == 0) {
                         m_SelectedIndex = -1;
                         m_NowSelect = m_RandomOrder;
                    }else */if (m_SelectedIndex > 0) {
                        m_SelectedIndex -= 1;
                        m_NowSelect = m_Items.at(m_SelectedIndex);
                    }
                    }else if (Util::Input::IsKeyDown(Util::Keycode::S) ||
                            Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
                        /*if (m_SelectedIndex == -1) {
                            m_SelectedIndex = 0;
                            m_NowSelect = m_Items.at(0);
                        }else */if (m_SelectedIndex < ListLength-1) {
                            m_SelectedIndex += 1;
                            m_NowSelect = m_Items.at(m_SelectedIndex);
                        }
                            }
                m_NowSelect->Focus();
            }else {
                LOG_ERROR("觸發空白項目");
                throw std::invalid_argument("The list should NOT be without selected items.");
            }

            if (/*((m_SelectedIndex == -1) ^ (m_NowSelect == m_RandomOrder)) or */(m_SelectedIndex != -1 and m_NowSelect != m_Items.at(m_SelectedIndex))) {
                LOG_WARN("SelectedIndex doesn't match NowSelect");
            }
        }

        if (play || Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            // if (m_NowSelect != m_RandomOrder) {
            //     return ScreenState::Playground;
            // }
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrHit);
            Button::s_IsKeyboardMode = true;
            SDL_ShowCursor(SDL_DISABLE);
        }
        // m_RandomOrder->Update();
        for (int i = 0 ; i < ListLength ; i++) {
            m_Items.at(i)->Update();
        }
    }

    m_Renderer.Update();

    return ScreenState::LevelList;
}
