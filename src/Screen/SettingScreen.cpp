//
// Created by cheese on 2026/5/2.
//

#include "Screen/SettingScreen.hpp"

SettingScreen::SettingScreen() {
    m_CurrentPage = Page::Volume;

    m_Background = std::make_shared<ImageObject>("Resources/Image/Setting/Background.png");
    m_Background->m_Transform.scale = glm::vec2(50, 25);
    m_Background->SetZIndex(0);
    m_Renderer.AddChild(m_Background);

    for (int i = 0 ; i < 2 ; i++) {
        auto line = std::make_shared<ImageObject>("Resources/Image/OptionBackground/Selected.png");
        m_LeftLines.push_back(line);
        line->m_Transform.scale = glm::vec2(20.0f, 0.1f);
    }

    // m_VolumePage = std::make_shared<Button>("Resources/Font/TSTC.ttf", 25, "音量");
    // m_VolumePage->m_Transform.translation = ;

}

void SettingScreen::Call(ScreenState WhoCalls) {

}

void SettingScreen::HangUp() {

}

ScreenState SettingScreen::Update() {

    // 它不應該被畫面選擇器接收
    return ScreenState::Exit;
}
