//
// Created by cheese on 2026/5/2.
//

#include "Screen/SettingScreen.hpp"

#include "Manager/MusicPlayerManager.hpp"

SettingScreen::SettingScreen() {
    m_CurrentPage = Page::Volume;

    m_Background = std::make_shared<ImageObject>("../Resources/Image/Setting/Background.png");
    m_Background->m_Transform.scale = glm::vec2(50, 25);
    m_Background->SetZIndex(0);
    m_Renderer.AddChild(m_Background);

    for (int i = 0 ; i < 2 ; i++) {
        auto line = std::make_shared<ImageObject>("../Resources/Image/OptionBackground/Selected.png");
        m_LeftLines.push_back(line);
        line->m_Transform.scale = glm::vec2(15.0f, 0.1f);
    }

    m_VolumePage = std::make_shared<MergeButton>(LeftWhere, 25, "音量", selected);
    LeftWhere.y -= m_VolumePage->GetSize().y + gap;
    m_Renderer.AddChild(m_VolumePage);

    m_Return = std::make_shared<MergeButton>(LeftWhere, 25, "返回", none);
    LeftWhere.y -= m_Return->GetSize().y + gap;
    m_Renderer.AddChild(m_Return);

    m_Restart = std::make_shared<MergeButton>(LeftWhere, 25, "重新開始", none);
    LeftWhere.y -= m_Restart->GetSize().y + gap;
    m_Renderer.AddChild(m_Restart);

    m_LeaveToList = std::make_shared<MergeButton>(LeftWhere, 25, "離開", none);
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
    });
    m_SFXSlider->SetValue(MusicPlayerManager::Setting().GetSFXVolume()*100/5);
    RightWhere.y -= m_SFXLabel->GetScaledSize().y + gap;
    m_Renderer.AddChild(m_SFXSlider);
}

void SettingScreen::Call(ScreenState WhoCalls) {
    if (WhoCalls == ScreenState::Playground) {
        m_Restart->SetShow(true);
        m_LeaveToList->SetShow(true);
    }else {
        m_Restart->SetShow(false);
        m_LeaveToList->SetShow(false);
    }
}

void SettingScreen::HangUp() {

}

ScreenState SettingScreen::Update() {

    m_VolumePage->Update();
    m_Return->Update();
    m_Restart->Update();
    m_LeaveToList->Update();
    m_BGMSlider->Update();
    m_SFXSlider->Update();
    m_Renderer.Update();
    // 它不應該被畫面選擇器接收
    return ScreenState::Exit;
}
