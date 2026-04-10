//
// Created by cheese on 2026/3/27.
//

#include "../include/PlaygroundScreen.hpp"
PlaygroundScreen::PlaygroundScreen(Levels level){
    SDL_ShowCursor(SDL_DISABLE);
    LOG_DEBUG("PlaygroundScreen::PlaygroundScreen");
    switch (level) {
        case Levels::Chronos:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.mp3";
            BPM = static_cast<float>(SongsBPM::Chronos);
            m_TimeLine = std::make_shared<TimeLine>(m_SongPath, BPM);
            break;
        default:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.mp3";
            BPM = static_cast<float>(SongsBPM::Chronos);
            m_TimeLine = std::make_shared<TimeLine>(m_SongPath, BPM);
            break;
    }

    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap, m_SongPath, BPM);
    m_LevelSpawner->SetZIndex(40);
    m_LevelSpawner->Start();
    m_Renderer.AddChild(m_LevelSpawner);

    m_TimeLine->Start();
    m_TimeLine->SetVolume(50);
}

ScreenState PlaygroundScreen::Update() {

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        if (m_TimeLine->IsPlaying()) {
            m_TimeLine->Pause();
        }
        else{
            m_TimeLine->Resume();
        }
    }
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        m_TimeLine->Stop();
    }

    m_TimeLine->Update();


    // ==========================================
    // 3. 節拍顯示debug用
    // ==========================================

    ImGui::Begin("test");
    ImGui::SetWindowPos({200, 300});
    ImGui::Text("Beats:%f", m_TimeLine->GetBeats());
    ImGui::End();

    m_LevelSpawner->Update(m_TimeLine->GetBeats());

    m_Renderer.Update();


    /*
    for (int i = 0; i < static_cast<int>(m_TestingPower.size()); i++) {
        if (!m_TestingPower[i]->IsFinished()) {
            m_TestingPower[i]->Update();
        }
        else {
            m_TestingPower.erase(m_TestingPower.begin() + i);
        }
    }
    */

/*
    if (m_LevelSpawner && !m_LevelSpawner->IsFinished()) {
        m_LevelSpawner->Update();
    }
*/

    return ScreenState::Playground;
}
