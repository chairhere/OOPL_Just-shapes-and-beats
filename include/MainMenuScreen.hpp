//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#include "Button.hpp"
#include "ScreenState.hpp"
#include "Screen.hpp"
#include "FadeLayer.hpp"

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
    std::shared_ptr<FadeLayer> m_FadeLayerIn, m_FadeLayerOut;
    std::shared_ptr<Util::GameObject> m_WarningImage;
    OpeningState m_OpeningState = OpeningState::Warning;

    std::vector<Util::Color> m_Colors = {{0, 0, 0, 255}, {0, 0, 0, 0}};
    std::vector<float> m_Duration = {0, 1500};
    std::vector<glm::vec2> m_Position = {{0.0f, 0.0f}, {0.0f, 0.0f}};
    std::vector<glm::vec2> m_Scale = {{WINDOW_WIDTH, WINDOW_HEIGHT}, {WINDOW_WIDTH, WINDOW_HEIGHT}};
};

#endif //JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP