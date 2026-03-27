//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP
#include "Button.hpp"
#include "Levels.hpp"
#include "Screen.hpp"
#include "FadeLayer.hpp"

class MainMenuScreen : public Screen{
public:
    ~MainMenuScreen() = default;

    MainMenuScreen();

    Levels Update() override;

private:
    bool playlist = false;
    bool exit = false;
    std::shared_ptr<Button> m_ButtonPlay, m_ButtonExit;
    std::shared_ptr<Button> m_Title, m_Hint;  //Icon
    std::shared_ptr<Button> m_NowSelect = nullptr;
    std::shared_ptr<FadeLayer> m_FadeLayerIn, m_FadeLayerOut;
    std::shared_ptr<Util::GameObject> m_WarningImage;
};

#endif //JUST_SHAPES_AND_BEATS_MAINMENUSCREEN_HPP