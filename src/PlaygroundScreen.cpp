//
// Created by cheese on 2026/3/27.
//

#include "../include/PlaygroundScreen.hpp"
PlaygroundScreen::PlaygroundScreen(Levels level){
    SDL_ShowCursor(SDL_DISABLE);
    switch (level) {
        case Levels::Chronos:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.mp3";
            BPM = static_cast<float>(SongsBPM::Chronos);
            break;
        default:
            m_BeatMap += "Chronos.json";
            m_SongPath += "Chronos.mp3";
            BPM = static_cast<float>(SongsBPM::Chronos);
            break;
    }

    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap, m_SongPath, BPM);
}

ScreenState PlaygroundScreen::Update() {

    for (int i = 0; i < static_cast<int>(m_TestingPower.size()); i++) {
        if (!m_TestingPower[i]->IsFinished()) {
            m_TestingPower[i]->Update();
        }
        else {
            m_TestingPower.erase(m_TestingPower.begin() + i);
        }
    }

/*
    if (m_LevelSpawner && !m_LevelSpawner->IsFinished()) {
        m_LevelSpawner->Update();
    }
*/

    return ScreenState::Playground;
}
