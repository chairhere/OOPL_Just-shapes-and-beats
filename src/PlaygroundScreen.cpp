//
// Created by cheese on 2026/3/27.
//

#include "../include/PlaygroundScreen.hpp"

#include "MusicPlayerManager.hpp"
#include "SongData.hpp"
#include "SongList.hpp"
#include "SongsBPM.hpp"

OpeningAnimateScreen::OpeningAnimateScreen(Levels level){
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    if (not debug)
        SDL_ShowCursor(SDL_DISABLE);
    LOG_DEBUG("PlaygroundScreen::PlaygroundScreen");
    SongData data;
    switch (level) {
        case Levels::Chronos:
            data = SongList::GetSongByName(Levels::Chronos);
            m_BeatMap = data.BeatMap;
            m_SongPath = data.AudioPath;
            BPM = static_cast<float>(data.BPM);
            MusicPlayerManager::Setting().Switch(data.Level);
            MusicPlayerManager::Setting().InfLoop(false);
            break;
        default:
            m_BeatMap += "Test.json";
            m_SongPath += "Chronos.wav";
            BPM = static_cast<float>(SongsBPM::Chronos);
            MusicPlayerManager::Setting().Switch(Levels::Chronos);
            MusicPlayerManager::Setting().InfLoop(false);
            break;
    }

    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap);
    m_LevelSpawner->SetZIndex(30);
    m_LevelSpawner->Start();
    //m_Renderer.AddChild(m_LevelSpawner);

    m_Player = std::make_shared<Player>();
    m_Player->SetPosition(glm::vec2(0.0f, 0.0f));
    m_Player->SetZIndex(50);
    //m_Player->SetVisible(false);
    m_Renderer.AddChild(m_Player);
    MusicPlayerManager::Setting().Play();
}

ScreenState OpeningAnimateScreen::Update() {
    m_PlayerDie = m_Player->Moving();

    if (Util::Input::IsKeyDown(Util::Keycode::TAB)) {
        debug ^= 1;
        if (debug) {
            SDL_ShowCursor(SDL_ENABLE);
        }else {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }
    if (not undead && m_PlayerDie) {
        switch (m_DieStage) {
            case DieStage::Alive:
                m_Player->Die();  //死亡
                m_DieStage = DieStage::SlowDown;
                break;
            case DieStage::SlowDown:
                if (m_MusicSpeed > 0.01) {  //音樂速度放慢
                    m_MusicSpeed -= 0.01f;
                    MusicPlayerManager::Setting().SetSpeed(m_MusicSpeed);
                }else {  //音樂速度停了
                    MusicPlayerManager::Setting().ReverseAt(MusicPlayerManager::Setting().GetBeats());
                    m_DieStage = DieStage::Rewinding;
                }
                break;
            case DieStage::Rewinding:
                if (m_MusicSpeed < 1.7f) {
                    m_MusicSpeed += 0.02;
                    MusicPlayerManager::Setting().SetSpeed(m_MusicSpeed);
                }else {
                    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap);
                    m_LevelSpawner->Start();
                    m_MusicSpeed = 1.0f;
                    MusicPlayerManager::Setting().SetSpeed(m_MusicSpeed);
                    MusicPlayerManager::Setting().PlayAt(0.0f);
                    m_Player->Revive();
                    m_DieStage = DieStage::Alive;
                }
                break;
        }
    }
    if (debug) {
        if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
            if (MusicPlayerManager::Setting().IsPause()) {
                MusicPlayerManager::Setting().Play();
            }else {
                MusicPlayerManager::Setting().Pause();
            }
        }
        if (Util::Input::IsKeyDown(Util::Keycode::P)) {
            std::string log = "current_beat: ";
            log.append(std::to_string(MusicPlayerManager::Setting().GetBeats()));
            LOG_DEBUG(log);
        }
    }


    m_LevelSpawner->Update(MusicPlayerManager::Setting().GetBeats(), m_Player->GetPosition());
    m_Player->Shake(m_LevelSpawner->GetCurrentShakeOffset());

    m_LevelSpawner->DrawAll();

    if (m_LevelSpawner->IsColliding() && not invincible) {
        m_Player->Hit();
    }

    // ==========================================
    // 3. 節拍顯示debug用
    // ==========================================

    if (debug) {
        ImGui::Begin("test");
        ImGui::SetWindowPos({200, 300});
        ImGui::Text("Beats:%f", MusicPlayerManager::Setting().GetBeats());
        static float v = 0.0f;
        ImGui::SliderFloat("Beats", &v, 0.0f, MusicPlayerManager::Setting().GetTotalBeats());
        if (ImGui::Button("Play at", ImVec2(50, 20))) {
            MusicPlayerManager::Setting().PlayAt(v);
            m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap);
            m_LevelSpawner->Start();
        }
        ImGui::Checkbox("Undead", &undead);
        ImGui::Checkbox("Invincible", &invincible);
        ImGui::Separator();
        ImGui::Text("FPS:%f", 1000.0F / Util::Time::GetDeltaTimeMs());
        ImGui::Text("Obstacles:%d", m_LevelSpawner->GetObstaclesCount());
        ImGui::End();
    }


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
