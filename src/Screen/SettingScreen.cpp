//
// Created by cheese on 2026/5/2.
//

#include "Screen/SettingScreen.hpp"

#include "App.hpp"
#include "Manager/MusicPlayerManager.hpp"

SettingScreen::SettingScreen() {
    m_CurrentPage = Page::Volume;

    m_Background = std::make_shared<ImageObject>("../Resources/Image/Setting/Background.png");
    m_Background->m_Transform.scale = glm::vec2(50, 25);
    m_Background->SetZIndex(-50);
    m_Renderer.AddChild(m_Background);

    m_VolumePage = std::make_shared<MergeButton>(LeftWhere, 25, "音量", none);
    m_VolumePage->SetFocusImage(selected);
    m_VolumePage->SetOnClick([this]() {
        if (m_NowSelect != m_VolumePage) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_NowSelect->Unfocus();
            this->m_NowSelect = m_VolumePage;
            this->m_NowSelect->Focus();
            this->m_SFXSelect = m_NowSelect;
        }
    });
    m_VolumePage->SetOnFocus([this]() {
        if (m_SFXSelect != m_VolumePage) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
        }
        this->m_NowSelect = m_VolumePage;
        this->m_SFXSelect = m_NowSelect;
    });
    m_VolumePage->HoverEnable(false);
    LeftWhere.y -= m_VolumePage->GetSize().y + gap;
    m_Renderer.AddChild(m_VolumePage);

    m_Return = std::make_shared<MergeButton>(LeftWhere, 25, "返回", none);
    m_Return->SetFocusImage(selected);
    m_Return->SetOnClick([this]() {
        if (m_NowSelect != m_Return) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_NowSelect->Unfocus();
            this->m_NowSelect = m_Return;
            this->m_NowSelect->Focus();
            this->m_SFXSelect = m_NowSelect;
        }else {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
            renum = 1;
        }
    });
    m_Return->SetOnFocus([this]() {
        if (m_SFXSelect != m_Return) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
        }
        this->m_NowSelect = m_Return;
        this->m_SFXSelect = m_NowSelect;
    });
    m_Return->HoverEnable(false);
    LeftWhere.y -= m_Return->GetSize().y + gap;
    m_Renderer.AddChild(m_Return);

    m_Restart = std::make_shared<MergeButton>(LeftWhere, 25, "重新開始", none);
    m_Restart->SetFocusImage(selected);
    m_Restart->SetOnClick([this]() {
        if (m_NowSelect != m_Restart) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_NowSelect->Unfocus();
            this->m_NowSelect = m_Restart;
            this->m_NowSelect->Focus();
            this->m_SFXSelect = m_NowSelect;
        }else {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
            renum = 2;
        }
    });
    m_Restart->SetOnFocus([this]() {
        if (m_SFXSelect != m_Restart) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
        }
        this->m_NowSelect = m_Restart;
        this->m_SFXSelect = m_NowSelect;
    });
    m_Restart->HoverEnable(false);
    LeftWhere.y -= m_Restart->GetSize().y + gap;
    m_Renderer.AddChild(m_Restart);

    m_LeaveToList = std::make_shared<MergeButton>(LeftWhere, 25, "離開", none);
    m_LeaveToList->SetFocusImage(selected);
    m_LeaveToList->SetOnClick([this]() {
        if (m_NowSelect != m_LeaveToList) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_NowSelect->Unfocus();
            this->m_NowSelect = m_LeaveToList;
            this->m_NowSelect->Focus();
            this->m_SFXSelect = m_NowSelect;
        }else {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
            renum = 3;
        }
    });
    m_LeaveToList->SetOnFocus([this]() {
        if (m_SFXSelect != m_LeaveToList) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
        }
        this->m_NowSelect = m_LeaveToList;
        this->m_SFXSelect = m_NowSelect;
    });
    m_LeaveToList->HoverEnable(false);
    LeftWhere.y -= m_LeaveToList->GetSize().y + gap;
    m_Renderer.AddChild((m_LeaveToList));

    m_VolumeLabel = std::make_shared<TextObject>(25, "聲音", Util::Color(0, 255, 255));
    m_VolumeLabel->m_Transform.translation = RightWhere;
    RightWhere.y -= m_VolumeLabel->GetScaledSize().y + gap;
    m_Renderer.AddChild(m_VolumeLabel);

    m_BGMLabel = std::make_shared<TextObject>(25, "音樂");
    m_BGMLabel->m_Transform.translation = RightWhere;
    m_Renderer.AddChild(m_BGMLabel);

    m_BGMSlider = std::make_shared<DiscreteSlider>(RightWhere.x+150, RightWhere.y);
    m_BGMSlider->SetOnValueChange([](int i) {
        MusicPlayerManager::Setting().SetBGMVolume(static_cast<float>(i)*5.0f/100.0f);
        MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
    });
    m_BGMSlider->SetValue(MusicPlayerManager::Setting().GetBGMBolume()*100/5);
    RightWhere.y -= m_BGMLabel->GetScaledSize().y + gap;
    m_Renderer.AddChild(m_BGMSlider);

    m_SFXLabel = std::make_shared<TextObject>(25, "音效");
    m_SFXLabel->m_Transform.translation = RightWhere;
    m_Renderer.AddChild(m_SFXLabel);

    m_SFXSlider = std::make_shared<DiscreteSlider>(RightWhere.x+150, RightWhere.y);
    m_SFXSlider->SetOnValueChange([](int i) {
        MusicPlayerManager::Setting().SetSFXVolume(static_cast<float>(i)*5.0f/100.0f);
        MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
    });
    m_SFXSlider->SetValue(MusicPlayerManager::Setting().GetSFXVolume()*100/5);
    RightWhere.y -= m_SFXLabel->GetScaledSize().y + gap;
    m_Renderer.AddChild(m_SFXSlider);

    m_NowSelect = m_Restart;

}

void SettingScreen::Call(ScreenState WhoCalls) {
    freeze = true;
    this->WhoCalls = WhoCalls;
    m_BGMSlider->SetValue(MusicPlayerManager::Setting().GetBGMBolume()*100/5);
    m_SFXSlider->SetValue(MusicPlayerManager::Setting().GetSFXVolume()*100/5);
    if (m_NowSelect != m_VolumePage) {
        m_NowSelect->Unfocus();
        m_NowSelect = m_VolumePage;
        m_NowSelect->Focus();
    }
    if (WhoCalls == ScreenState::Playground) {
        m_Restart->SetShow(true);
        m_LeaveToList->SetShow(true);
    }else {
        m_Restart->SetShow(false);
        m_LeaveToList->SetShow(false);
    }
}

void SettingScreen::HangUp() {
    freeze = false;
    MusicPlayerManager::Setting().Play();
}


ScreenState SettingScreen::Update() {

    //防Hover與Focus衝突
    if (Util::Input::IsMouseMoving()) {
        Button::s_IsKeyboardMode = false;
        SDL_ShowCursor(SDL_ENABLE);
    }
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
                if (m_NowSelect == m_Return) {
                    m_NowSelect = m_VolumePage;
                }else if (m_NowSelect == m_Restart) {
                    m_NowSelect = m_Return;
                }else if (m_NowSelect == m_LeaveToList) {
                    m_NowSelect = m_Restart;
                }
            }else if (
                Util::Input::IsKeyDown(Util::Keycode::S) ||
                Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
                if (m_NowSelect == m_VolumePage) {
                    m_NowSelect = m_Return;
                }else if (m_NowSelect == m_Return and WhoCalls == ScreenState::Playground) {
                    m_NowSelect = m_Restart;
                }else if (m_NowSelect == m_Restart and WhoCalls == ScreenState::Playground) {
                    m_NowSelect = m_LeaveToList;
                }
            }
            m_NowSelect->Focus();
        }else {
            LOG_ERROR("觸發空白項目");
            throw std::invalid_argument("The list should NOT be without selected items.");
        }
    }

    m_VolumePage->Update();
    m_Return->Update();
    m_Restart->Update();
    m_LeaveToList->Update();
    m_BGMSlider->Update();
    m_SFXSlider->Update();
    m_Renderer.Update();

    if (Util::Input::IsKeyDown(Util::Keycode::TAB)) {
        std::string log = "button pos(" + std::to_string(m_Return->m_Transform.translation.x) + ", " + std::to_string(m_Return->m_Transform.translation.y) + ")\nHovering: ";
        log.append(m_Return->isHovering() ? "true" : "false");
        LOG_DEBUG(log);
    }

    switch (renum) {
        case 1:
            renum = 0;
            return ScreenState::Exit;
        case 2:
            renum = 0;
            return ScreenState::Playground;
        case 3:
            renum = 0;
            return ScreenState::LevelList;
        default:
            return ScreenState::Main;
    }
}
