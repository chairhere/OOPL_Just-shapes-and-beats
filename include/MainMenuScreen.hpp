//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#include "Button.hpp"
#include "Screen.hpp"

class MainMenuScreen : public Screen{

    ~MainMenuScreen() = default;

    MainMenuScreen() {
        std::shared_ptr<Button> m_button_play, m_button_option, m_button_exit;
        //setting
        m_Renderer.AddChild(m_button_play);
        m_Renderer.AddChild(m_button_option);
        m_Renderer.AddChild(m_button_exit);
    }
};

#endif //JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP