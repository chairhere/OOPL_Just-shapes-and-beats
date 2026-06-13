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

    void RestartRequire();

    ScreenState Update() override;

    void Ending();
private:
    Util::Color m_ObstacleColor = {255, 33, 111};
    std::string m_BeatMap = "../Resources/BeatMap/";
    std::string m_SongPath = "../Resources/Audio/";
    std::shared_ptr<LevelSpawner> m_LevelSpawner;
    std::shared_ptr<FadeLayer> m_TestingPower;
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

    bool debug = true;
    bool debugLock = false;
    bool steady = true;
    bool firm = true;
    bool Is_End = false;

    std::vector<Util::Color> First_Color = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}};
    std::vector<float> First_Duration = {0.0f, 1500.0f, 4000.0f, 5500.0f};
    std::vector<float> First_Rotation = {0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<float> First_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    //std::vector<float> First_Vertices = {-0.288f, 0.5f, -0.577f, -0.0f, -0.288f, -0.5f, 0.288f, -0.5f, 0.577f, 0.0f, 0.288f, 0.5f};
    std::vector<glm::vec2> First_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> First_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};

    ScreenState m_NextState = ScreenState::Playground;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP