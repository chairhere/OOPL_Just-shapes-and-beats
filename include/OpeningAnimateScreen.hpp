//
// Created by LAB1223 on 2026/4/24.
//

#ifndef JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
#define JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP

#include "Screen.hpp"
#include "ScreenState.hpp"
#include "FadeLayer.hpp"
#include "CustomColorShape.hpp"
#include "MusicPlayerManager.hpp"
#include "SongData.hpp"
#include "SongList.hpp"
#include "SongsBPM.hpp"
#include "Levels.hpp"
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

    std::shared_ptr<FadeLayer> m_FadeLayerIn;
    std::shared_ptr<Util::GameObject> m_WarningImage, m_OpeningTriangle, m_OpeningMelody, m_OpeningAnd, m_OpeningLogo;


    std::vector<Util::Color> First_Color = {{0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}};
    std::vector<float> First_Duration = {0.0f, 5000.0f, 7500.0f, 10500.0f, 13000.0f, 16000.0f};
    std::vector<float> First_Rotation = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<float> First_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    //std::vector<float> First_Vertices = {-0.288f, 0.5f, -0.577f, -0.0f, -0.288f, -0.5f, 0.288f, -0.5f, 0.577f, 0.0f, 0.288f, 0.5f};
    std::vector<glm::vec2> First_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> First_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};

    float BPM;

    float m_ElapsedTime = 0.0f;

    int m_AnimateState = 1;

    SongData data;
};

#endif //JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
