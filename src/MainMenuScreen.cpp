//
// Created by cheese on 2026/3/20.
//

#include "MainMenuScreen.hpp"
#include "ScreenState.hpp"
#include <SDL_events.h>

#include "MusicPlayerManager.hpp"

MainMenuScreen::MainMenuScreen() {
    // === 音樂 ===
    MusicPlayerManager::Setting().Switch(Levels::MainMenu);
    MusicPlayerManager::Setting().InfLoop(true);

    // === Play 按鈕 ===
    m_ButtonPlay = std::make_shared<Button>("../Resources/Image/MainScreenButton/SongListButton.png");

    m_ButtonPlay->SetFocusImage("../Resources/Image/MainScreenButton/SongListButton(Selected).png");

    m_ButtonPlay->SetOnHovering([this]() {
        if (m_SFXSelect != m_ButtonPlay) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_SFXSelect = m_ButtonPlay;
        }
        this->m_NowSelect = m_ButtonPlay;
    });
    m_ButtonPlay->SetOnFocus([this]() {
        if (m_SFXSelect != m_ButtonPlay) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_SFXSelect = m_ButtonPlay;
        }
        this->m_NowSelect = m_ButtonPlay;
    });
    m_ButtonPlay->SetOffEvent([this]() {
        this->m_SFXSelect = nullptr;
        this->m_NowSelect = nullptr;
    });
    m_ButtonPlay->SetOnClick([this]() {
        MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
        playlist = true;
    });

    m_ButtonPlay->m_Transform.translation = glm::vec2(500, 10);
    m_ButtonPlay->SetZIndex(50);
    m_Renderer.AddChild(m_ButtonPlay);

    // === Exit 按鈕 ===
    m_ButtonExit = std::make_shared<Button>("../Resources/Image/MainScreenButton/ExitButton.png");

    m_ButtonExit->SetFocusImage("../Resources/Image/MainScreenButton/ExitButton(Selected).png");

    m_ButtonExit->SetOnHovering([this]() {
        if (m_SFXSelect != m_ButtonExit) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_SFXSelect = m_ButtonExit;
        }
        this->m_NowSelect = m_ButtonExit;
    });
    m_ButtonExit->SetOnFocus([this]() {
        if (m_SFXSelect != m_ButtonExit) {
            MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtSelect);
            this->m_SFXSelect = m_ButtonExit;
        }
        this->m_NowSelect = m_ButtonExit;
    });
    m_ButtonExit->SetOffEvent([this]() {
        this->m_SFXSelect = nullptr;
        this->m_NowSelect = nullptr;
    });
    m_ButtonExit->SetOnClick([this]() {
        MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::BtClick);
        // 發送 SDL 退出事件
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
        exit = true;
    });

    m_ButtonExit->m_Transform.translation = glm::vec2(600, -200);
    m_ButtonExit->SetZIndex(50);
    m_Renderer.AddChild(m_ButtonExit);

    m_Title = std::make_shared<Button>("../Resources/Image/Icon/Just_Shapes_26_Beats_logo.png");
    m_Title->m_Transform.translation = glm::vec2(-350, 200);
    m_Title->m_Transform.scale = glm::vec2(0.5, 0.5);
    m_Title->SetZIndex(50);
    m_Renderer.AddChild(m_Title);

    m_Hint = std::make_shared<Button>("../Resources/Image/MainScreenButton/Hint.png");
    m_Hint->m_Transform.translation = glm::vec2(400, -370);
    m_Hint->SetZIndex(50);
    m_Renderer.AddChild(m_Hint);

}

ScreenState MainMenuScreen::Update() {
    //防Hover與Focus衝突
    if (Util::Input::IsMouseMoving()) {
        Button::s_IsKeyboardMode = false;
        SDL_ShowCursor(SDL_ENABLE);

        if (m_NowSelect) {
            m_NowSelect->Unfocus();
        }
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
                Util::Input::IsKeyDown(Util::Keycode::S) ||
                Util::Input::IsKeyDown(Util::Keycode::UP) ||
                Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
                if (m_NowSelect == m_ButtonPlay) {
                    m_NowSelect = m_ButtonExit;
                }else if (m_NowSelect == m_ButtonExit) {
                    m_NowSelect = m_ButtonPlay;
                }
            }
            m_NowSelect->Focus();
        }else {
            if (Util::Input::IsKeyDown(Util::Keycode::W) ||
                Util::Input::IsKeyDown(Util::Keycode::S) ||
                Util::Input::IsKeyDown(Util::Keycode::UP) ||
                Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
                m_ButtonPlay->Focus();
            }
        }
    }

    if (m_FadeLayerIn && !m_FadeLayerIn->IsFinished()) {
        m_FadeLayerIn->Update(); // 推進 1.5 秒的計時與透明度變化
        //LOG_DEBUG("Fade Layer Updated");
    }
    // 當動畫播完後，將其從渲染清單移除並釋放資源
    else if (m_FadeLayerIn && m_FadeLayerIn->IsFinished()) {
        m_Renderer.RemoveChild(m_FadeLayerIn); // 從畫面中剔除 [5]
        m_FadeLayerIn = nullptr;               // 清空指標，釋放記憶體
        //LOG_DEBUG("Fade Layer finished");
    }
     /*
     else if (m_FadeLayerOut && !m_FadeLayerOut->IsFinished()) {
         m_FadeLayerOut->Update(); // 推進 1.5 秒的計時與透明度變化
         //LOG_DEBUG("Fade Layer Updated");
     }
     // 當動畫播完後，將其從渲染清單移除並釋放資源
     else if (m_FadeLayerOut && m_FadeLayerOut->IsFinished()) {
         m_Renderer.RemoveChild(m_FadeLayerOut); // 從畫面中剔除 [5]
         m_Renderer.RemoveChild(m_WarningImage);
         m_FadeLayerOut = nullptr;               // 清空指標，釋放記憶體
         //LOG_DEBUG("Fade Layer finished");
     }
// */

    // 更新畫面與按鈕邏輯
    m_Renderer.Update();
    m_ButtonPlay->Update();
    m_ButtonExit->Update();

    if (playlist) {
        return ScreenState::LevelList;
    }else if (exit) {
        return ScreenState::Exit;
    }

    return ScreenState::Main;
}