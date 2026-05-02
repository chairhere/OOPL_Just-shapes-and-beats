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
#include "FadeLayer.hpp"


class OpeningAnimateScreen : public Screen {
public:
    ~OpeningAnimateScreen() override = default;

    explicit OpeningAnimateScreen(Levels level);

    ScreenState Update() override;
private:
    Util::Color m_ObstacleColor = {255, 33, 111};
    std::string m_BeatMap = "../Resources/BeatMap/";
    std::string m_SongPath = "../Resources/Audio/";
    std::shared_ptr<LevelSpawner> m_LevelSpawner;
    std::vector<std::shared_ptr<FadeLayer>> m_TestingPower;
    std::shared_ptr<Player> m_Player;

    bool m_PlayerDie = false;
    float m_MusicSpeed = 1.0f;
    enum class DieStage {
        Alive,
        SlowDown,
        Rewinding
    } m_DieStage = DieStage::Alive;

    float BPM;

    bool debug = false;
    bool undead = false;
    bool invincible = false;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP