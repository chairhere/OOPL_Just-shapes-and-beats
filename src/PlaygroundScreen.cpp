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
            BPM = SongsBPM::Chronos;
            break;
    }

    m_LevelSpawner = std::make_shared<LevelSpawner>(m_BeatMap, m_SongPath);
}

ScreenState PlaygroundScreen::Update() {
    return ScreenState::Playground;
    if (m_LevelSpawner && !m_LevelSpawner->IsFinished()) {
        m_LevelSpawner->Update();
    }
}
