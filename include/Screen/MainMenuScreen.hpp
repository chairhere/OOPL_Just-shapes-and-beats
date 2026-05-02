//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#include "EventObject/Button.hpp"
#include "Tool/ScreenState.hpp"
#include "Screen/Screen.hpp"
#include "Material/FadeLayer.hpp"

class MainMenuScreen : public Screen{
public:

    enum class OpeningState {
        Warning,
        Opening,
        MainMenu
    };

    ~MainMenuScreen() = default;

    MainMenuScreen();

    ScreenState Update() override;

private:
    bool playlist = false;
    bool exit = false;
    std::shared_ptr<Button> m_ButtonPlay, m_ButtonExit;
    std::shared_ptr<Button> m_Title, m_Hint;  //Icon
    std::shared_ptr<Button> m_NowSelect = nullptr;
    std::shared_ptr<Button> m_SFXSelect = nullptr;
    std::shared_ptr<FadeLayer> m_FadeLayerIn, m_FadeLayerOut;
    std::shared_ptr<Util::GameObject> m_WarningImage;
    OpeningState m_OpeningState = OpeningState::Warning;

    std::vector<Util::Color> First_Color = {{0, 0, 0, 255}, {0, 0, 0, 255}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 255}, {0, 0, 0, 255}};
    std::vector<float> First_Duration = {0.0f, 5000.0f, 7500.0f, 10500.0f, 13000.0f, 16000.0f};
    std::vector<float> First_Rotation = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::vector<float> First_Vertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    //std::vector<float> First_Vertices = {-0.288f, 0.5f, -0.577f, -0.0f, -0.288f, -0.5f, 0.288f, -0.5f, 0.577f, 0.0f, 0.288f, 0.5f};
    std::vector<glm::vec2> First_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> First_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};
};

#endif //JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP