//
// Created by cheese on 2026/5/2.
//

#include "Screen/SettingScreen.hpp"

SettingScreen::SettingScreen() {
    m_CurrentPage = Page::Volume;

    m_Background = std::make_shared<ImageObject>("../Resources/Image/Setting/Background.png");
    m_Background->m_Transform.scale = glm::vec2(50, 25);
    m_Background->SetZIndex(0);
    m_Renderer.AddChild(m_Background);

    for (int i = 0 ; i < 2 ; i++) {
        auto line = std::make_shared<ImageObject>("Resources/Image/OptionBackground/Selected.png");
        m_LeftLines.push_back(line);
        line->m_Transform.scale = glm::vec2(15.0f, 0.1f);
    }

    m_VolumePage = std::make_shared<MergeButton>(LeftWhere, 25, "音量", "../Resources/Image/OptionBackground/None.png");
    LeftWhere += m_VolumePage->GetSize().y + gap;
    m_Renderer.AddChild(m_VolumePage);

    m_VolumeLable = std::make_shared<TextObject>(25, "聲音", Util::Color(0, 255, 255));
    m_VolumeLable->m_Transform.translation = RightWhere;
    RightWhere += m_VolumeLable->GetScaledSize().y + gap;
    m_Renderer.AddChild(m_VolumeLable);



}

void SettingScreen::Call(ScreenState WhoCalls) {

}

void SettingScreen::HangUp() {

}

ScreenState SettingScreen::Update() {
    m_Renderer.Update();
    // 它不應該被畫面選擇器接收
    return ScreenState::Exit;
}
