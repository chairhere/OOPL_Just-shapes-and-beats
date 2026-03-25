//
// Created by cheese on 2026/3/20.
//

#include "MainMenuScreen.hpp"

#include "Levels.hpp"

MainMenuScreen::MainMenuScreen() {
    m_button_play = std::make_shared<Button>("../Resources/Image/MainScreenButton/SongListButton.png");
    m_button_play->SetOnHovering([this]() {
        m_button_play->SetImage("../Resources/Image/MainScreenButton/SongListButton(Selected).png");
    });
    m_button_play->SetOnFocus(m_button_play->OnHoverEvent());
    m_button_play->SetOnClick([this]() {
        playlist = true;
    });
    m_button_play->m_Transform.translation = glm::vec2(300, 10);
    m_button_play->SetZIndex(50);
    m_Renderer.AddChild(m_button_play);

    m_button_exit = std::make_shared<Button>("../Resources/Image/MainScreenButton/ExitButton.png");
    m_button_exit->SetOnHovering([this]() {
        m_button_exit->SetImage("../Resources/Image/MainScreenButton/ExitButton(Selected).png");
    });
    m_button_exit->SetOnFocus(m_button_exit->OnHoverEvent());
    m_button_exit->SetOnClick([this]() {
        exit = true;
    });
    m_button_exit->m_Transform.translation = glm::vec2(400, -200);
    m_button_exit->SetZIndex(50);
    m_Renderer.AddChild(m_button_exit);
}

Levels MainMenuScreen::Update() {
    // // 1. 如果偵測到滑鼠正在移動，顯示滑鼠游標
    // if (Util::Input::IsMouseMoving()) {
    //     SDL_ShowCursor(SDL_ENABLE);
    // }
    //
    // // 2. 如果玩家按下了任何鍵盤導航鍵 (例如 W, S, 上, 下, Enter)，隱藏滑鼠游標
    // // 您可以根據您的遊戲操作鍵來增加 IsKeyDown 的判斷
    // if (Util::Input::IsKeyDown(Util::Keycode::W) ||
    //     Util::Input::IsKeyDown(Util::Keycode::S) ||
    //     Util::Input::IsKeyDown(Util::Keycode::UP) ||
    //     Util::Input::IsKeyDown(Util::Keycode::DOWN) ||
    //     Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
    //
    //     SDL_ShowCursor(SDL_DISABLE);
    // }
    m_Renderer.Update();
    m_button_play->Update();
    m_button_exit->Update();
    if (playlist) {
        return Levels::LevelList;
    }else if (exit) {
        // 1. 建立一個 SDL 事件結構體
        SDL_Event quitEvent;

        // 2. 將事件類型設定為「退出」
        quitEvent.type = SDL_QUIT;

        // 3. 把這個事件推送到系統的事件佇列中
        SDL_PushEvent(&quitEvent);
    }

    return Levels::Main;
}
