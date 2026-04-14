//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP

#include "Screen.hpp"
#include "ScreenState.hpp"
#include "LevelSpawner.hpp"
#include "Levels.hpp"
#include "Player.hpp"
#include "Util/Color.hpp"
#include "SongsBPM.hpp"
#include "FadeLayer.hpp"
#include "TimeLine.hpp"
#include "Util/Input.hpp"
#include "imgui.h"
#include "MusicPlayerManager.hpp"


class PlaygroundScreen : public Screen {
public:
    ~PlaygroundScreen() override = default;

    explicit PlaygroundScreen(Levels level);

    ScreenState Update() override;
private:
    Util::Color m_ObstacleColor = {255, 33, 111};
    std::string m_BeatMap = "../Resources/BeatMap/";
    std::string m_SongPath = "../Resources/Audio/";
    std::shared_ptr<LevelSpawner> m_LevelSpawner;
    std::vector<std::shared_ptr<FadeLayer>> m_TestingPower;
    std::shared_ptr<Player> m_Player;

    int BPM;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP