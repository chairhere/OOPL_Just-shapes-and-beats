//
// Created by cheese on 2026/3/20.
//

#include "MainMenuScreen.hpp"
#include "Levels.hpp"
#include <SDL_events.h>

MainMenuScreen::MainMenuScreen() {
    // === Play 按鈕 ===
    m_button_play = std::make_shared<Button>("../Resources/Image/MainScreenButton/SongListButton.png");

    // 【修改點 1】直接告訴按鈕 Hover 時要用哪張圖，不要在 Lambda 裡面自己 Call SetImage！
    m_button_play->SetHoverImage("../Resources/Image/MainScreenButton/SongListButton(Selected).png");

    // (不用再設定 SetOnFocus，Button 內部會自動共用 HoverImage！)

    m_button_play->SetOnClick([this]() { playlist = true; });
    m_button_play->m_Transform.translation = glm::vec2(300, 10);
    m_button_play->SetZIndex(50);
    m_Renderer.AddChild(m_button_play);

    // === Exit 按鈕 ===
    m_button_exit = std::make_shared<Button>("../Resources/Image/MainScreenButton/ExitButton.png");

    // 【修改點 2】同樣直接設定 Hover 圖片
    m_button_exit->SetHoverImage("../Resources/Image/MainScreenButton/ExitButton(Selected).png");

    m_button_exit->SetOnClick([this]() { exit = true; });
    m_button_exit->m_Transform.translation = glm::vec2(400, -200);
    m_button_exit->SetZIndex(50);
    m_Renderer.AddChild(m_button_exit);
}

Levels MainMenuScreen::Update() {
    // 【修改點 3】使用我們討論的最佳解：鍵盤模式防護 (完美取代隱藏滑鼠游標)
    if (Util::Input::IsMouseMoving()) {
        Button::s_IsKeyboardMode = false;
    }

    // 檢查導航鍵 (您原本註解掉的判斷)
    if (Util::Input::IsKeyDown(Util::Keycode::W) ||
        Util::Input::IsKeyDown(Util::Keycode::S) ||
        Util::Input::IsKeyDown(Util::Keycode::UP) ||
        Util::Input::IsKeyDown(Util::Keycode::DOWN) ||
        Util::Input::IsKeyDown(Util::Keycode::RETURN)) {

        Button::s_IsKeyboardMode = true;
    }

    // 更新畫面與按鈕邏輯
    m_Renderer.Update();
    m_button_play->Update();
    m_button_exit->Update();

    // 狀態切換判斷
    if (playlist) {
        return Levels::LevelList;
    } else if (exit) {
        // 發送 SDL 退出事件
        SDL_Event quitEvent;
        quitEvent.type = SDL_QUIT;
        SDL_PushEvent(&quitEvent);
    }

    return Levels::Main;
}