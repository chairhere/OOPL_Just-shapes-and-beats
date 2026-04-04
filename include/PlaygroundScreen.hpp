//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP

#include "Screen.hpp"
#include "ScreenState.hpp"
#include "LevelSpawner.hpp"
#include "imgui.h"
#include "Levels.hpp"
#include "Util/Color.hpp"
#include "SongsBPM.hpp"


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

    SongsBPM BPM;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP