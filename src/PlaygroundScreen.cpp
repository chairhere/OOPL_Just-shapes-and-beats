//
// Created by cheese on 2026/3/27.
//

#include "../include/PlaygroundScreen.hpp"

#include "MusicPlayerManager.hpp"
#include "SongsBPM.hpp"

PlaygroundScreen::PlaygroundScreen(Levels level){
    SDL_ShowCursor(SDL_DISABLE);
    LOG_DEBUG("PlaygroundScreen::PlaygroundScreen");
    switch (level) {
        case Levels::Chronos:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.wav";
            BPM = static_cast<float>(SongsBPM::Chronos);
            MusicPlayerManager::Setting().Switch(Levels::Chronos);
            MusicPlayerManager::Setting().InfLoop(false);
            break;
        default:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.wav";
            BPM = static_cast<float>(SongsBPM::Chronos);
            MusicPlayerManager::Setting().InfLoop(false);
            break;
    }

    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap);
    m_LevelSpawner->SetZIndex(40);
    m_LevelSpawner->Start();
    m_Renderer.AddChild(m_LevelSpawner);

    m_Player = std::make_shared<Player>();
    m_Player->SetPosition(glm::vec2(0.0f, 0.0f));
    m_Player->SetZIndex(50);
    //m_Player->SetVisible(false);
    m_Renderer.AddChild(m_Player);
    MusicPlayerManager::Setting().Play();
}

ScreenState PlaygroundScreen::Update() {
    m_Player->Moving();

    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        MusicPlayerManager::Setting().Pause();
    }
    if (Util::Input::IsKeyDown(Util::Keycode::P)) {
        MusicPlayerManager::Setting().Play();
    }

    // ==========================================
    // 3. 節拍顯示debug用
    // ==========================================

    ImGui::Begin("test");
    ImGui::SetWindowPos({200, 300});
    ImGui::Text("Beats:%f", MusicPlayerManager::Setting().GetBeats());
    static float v = 0.0f;
    ImGui::SliderFloat("Beats", &v, 0.0f, MusicPlayerManager::Setting().GetTotalBeats());
    if (ImGui::Button("Play at", ImVec2(50, 20))) {
        MusicPlayerManager::Setting().PlayAt(v);
    }
    ImGui::End();

    m_LevelSpawner->Update(MusicPlayerManager::Setting().GetBeats(), m_Player->GetPosition());
    //m_LevelSpawner->Draw();

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
