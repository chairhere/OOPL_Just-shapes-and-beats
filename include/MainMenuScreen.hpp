//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#include "Button.hpp"
#include "Levels.hpp"
#include "Screen.hpp"

class MainMenuScreen : public Screen{
public:
    ~MainMenuScreen() = default;

    MainMenuScreen();

    Levels Update() override;

private:
    bool playlist = false;
    bool exit = false;
    std::shared_ptr<Button> m_button_play, m_button_exit;
};

#endif //JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP