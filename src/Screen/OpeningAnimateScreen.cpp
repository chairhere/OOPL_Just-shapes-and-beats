//
// Created by LAB1223 on 2026/4/24.
//
#include "Screen/OpeningAnimateScreen.hpp"

OpeningAnimateScreen::OpeningAnimateScreen() {

    data = SongList::GetSongByName(Levels::MainMenu);
    m_BeatMap = data.BeatMap;
    m_SongPath = data.AudioPath;
    BPM = static_cast<float>(data.BPM);
    MusicPlayerManager::Setting().Switch(data.Level);
    MusicPlayerManager::Setting().InfLoop(false);

    m_FadeLayerIn = std::make_shared<FadeLayer>(First_Color, First_Duration, First_Position, First_Rotation, First_Scale, First_Vertices, false);
    m_FadeLayerIn->SetZIndex(70);
    m_Renderer.AddChild(m_FadeLayerIn);

    m_WarningImage = std::make_shared<Util::GameObject>();
    m_WarningImage->SetDrawable(std::make_shared<Util::Image>("../Resources/Image/Others/Opening_Warning.png"));
    m_WarningImage->SetZIndex(60);
    m_WarningImage->m_Transform.scale = {0.8f, 0.8f};
    m_Renderer.AddChild(m_WarningImage);

    m_OpeningAnd = std::make_shared<Util::GameObject>();
    m_OpeningAnd->SetDrawable((std::make_shared<Util::Image>("../Resources/Others/OpeningAnd.png")));
    m_OpeningAnd->SetZIndex(50);
    m_OpeningAnd->m_Transform.scale = {1.0f, 1.0f};
    m_OpeningAnd->SetVisible(false);
    m_Renderer.AddChild(m_OpeningAnd);

    m_OpeningLogo = std::make_shared<Util::GameObject>();
    m_OpeningLogo->SetDrawable(std::make_shared<Util::Image>("../Resources/Icon/Just_Shapes_26_Beats_logo.png"));
    m_OpeningLogo->SetZIndex(50);
    m_OpeningLogo->m_Transform.scale = {1.0f, 1.0f};
    m_OpeningLogo->SetVisible(false);
    m_Renderer.AddChild(m_OpeningLogo);

    m_OpeningMelody = std::make_shared<Util::GameObject>();
    m_OpeningMelody->SetDrawable(std::make_shared<Util::Image>("../Resources/Others/OpeningTone_1.png"));
    m_OpeningMelody->SetZIndex(50);
    m_OpeningMelody->m_Transform.scale = {1.0f, 1.0f};
    m_OpeningMelody->SetVisible(false);
    m_Renderer.AddChild(m_OpeningMelody);

    m_OpeningTriangle = std::make_shared<Util::GameObject>();
    m_OpeningTriangle->SetDrawable(std::make_shared<Util::Image>("../Resources/Others/Icon_1.png"));
    m_OpeningTriangle->SetZIndex(50);
    m_OpeningTriangle->m_Transform.scale = {1.0f, 1.0f};
    m_OpeningTriangle->SetVisible(false);
    m_Renderer.AddChild(m_OpeningTriangle);

}

ScreenState OpeningAnimateScreen::Update() {

    if (m_AnimateState == 1 && m_FadeLayerIn && !m_FadeLayerIn->IsFinished()) {
        m_FadeLayerIn->Update(); // 推進 1.5 秒的計時與透明度變化
        //LOG_DEBUG("Fade Layer Updated");
    }
    // 當動畫播完後，將其從渲染清單移除並釋放資源
    else if (m_AnimateState == 1 && m_FadeLayerIn && m_FadeLayerIn->IsFinished()) {
        m_AnimateState = 2;
        m_Renderer.RemoveChild(m_FadeLayerIn); // 從畫面中剔除 [5]
        m_Renderer.RemoveChild(m_WarningImage);
        m_FadeLayerIn = nullptr;               // 清空指標，釋放記憶體
        m_WarningImage = nullptr;
        //LOG_DEBUG("Fade Layer finished");
    }
    else if (m_AnimateState == 2 )
        ;

    return ScreenState::OpeningAnimate;
}
