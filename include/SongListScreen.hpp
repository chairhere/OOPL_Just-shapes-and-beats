//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP

#include "Button.hpp"
#include "Screen.hpp"

//this also name as LevelList

class SongListScreen : public Screen {
public:
    ~SongListScreen() = default;

    SongListScreen();

    ScreenState Update() override;
private:
    std::vector<std::shared_ptr<Button>> m_ButtonLevels;
    std::shared_ptr<Button> m_NowSelect;
    int m_SelectedIndex = -1;
};

#endif //JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP