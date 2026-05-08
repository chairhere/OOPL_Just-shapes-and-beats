//
// Created by LAB1223 on 2026/4/24.
//

#ifndef JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
#define JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP

#include "Screen/Screen.hpp"
#include "Tool/ScreenState.hpp"
#include "Material/FadeLayer.hpp"
#include "Spawner/CustomColorShape.hpp"
#include "Manager/MusicPlayerManager.hpp"
#include "Tool/SongData.hpp"
#include "Tool/SongList.hpp"
#include "Tool/SongsBPM.hpp"
#include "Tool/Levels.hpp"
#include "Util/Color.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"

class OpeningAnimateScreen : public Screen {
public:
    ~OpeningAnimateScreen() override = default;

    explicit OpeningAnimateScreen();

    ScreenState Update() override;

private:
    std::string m_BeatMap = "../Resources/BeatMap/";
    std::string m_SongPath = "../Resources/Audio/";

    std::shared_ptr<FadeLayer> m_FadeLayerIn, m_FadeLayerMelody, m_FadeLayerAnd, m_FadeLayerLogo;
    std::shared_ptr<Util::GameObject> m_WarningImage, m_OpeningTriangle, m_OpeningMelody, m_OpeningAnd, m_OpeningLogo;


    std::vector<Util::Color> First_Color = {{0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}};
    std::vector<float> First_Duration = {0.0f, 5000.0f, 7500.0f, 10500.0f, 13000.0f, 16000.0f};
    std::vector<float> First_Rotation = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<float> First_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    //std::vector<float> First_Vertices = {-0.288f, 0.5f, -0.577f, -0.0f, -0.288f, -0.5f, 0.288f, -0.5f, 0.577f, 0.0f, 0.288f, 0.5f};
    std::vector<glm::vec2> First_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> First_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};


    std::vector<Util::Color> m_CoverColorOthers = {{0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    std::vector<Util::Color> m_CoverColorAnd = {{0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}};


    std::vector<float> m_CoverDurationLogo = {0.0f, 17000.0f, 17250.0f, 18500.0f};
    std::vector<float> m_CoverDurationAnd = {0.0f, 17250.0f, 17500.0f, 18500.0f, 19940.0f, 20190.0f};
    std::vector<float> m_CoverDurationMelody = {0.0f, 17500.0f, 17750.0f, 18500.0f};

    std::vector<float> m_CoverRotation = {0.0f, 0.0f, 0.0f};

    std::vector<glm::vec2> m_CoverPositionLogo = {{-150.0f, 0.0f}, {-150.0f, 0.0f}, {-150.0f, 0.0f}};
    std::vector<glm::vec2> m_CoverPositionAnd = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> m_CoverPositionMelody = {{150.0f, 0.0f}, {150.0f, 0.0f}, {150.0f, 0.0f}};

    std::vector<glm::vec2> m_CoverScale = {{100.0f, 100.0f}, {100.0f, 100.0f}, {100.0f, 100.0f}};


    float BPM;

    float m_ElapsedTime = 0.0f;

    int m_AnimateState = 1;

    SongData data;
};

#endif //JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
