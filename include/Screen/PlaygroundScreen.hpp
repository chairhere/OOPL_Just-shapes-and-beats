//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP

#include "Screen/Screen.hpp"
#include "Tool/ScreenState.hpp"
#include "Manager/LevelSpawner.hpp"
#include "Tool/Levels.hpp"
#include "Material/Player.hpp"
#include "Util/Color.hpp"
#include "Material/FadeLayer.hpp"


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

    std::vector<float> m_CheckPoints;
    float m_StartBeat = 0.0f;
    int m_WhichCheckPoint = 0;

    bool m_PlayerDie = false;
    float m_MusicSpeed = 1.0f;
    enum class DieStage {
        Alive,
        SlowDown,
        Rewinding
    } m_DieStage = DieStage::Alive;

    float BPM;

    bool debug = false;
    bool debugLock = false;
    bool steady = false;
    bool firm = false;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP