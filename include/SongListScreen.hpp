//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP

#include "Button.hpp"
#include "Screen.hpp"
#include "SongListItem.hpp"

//this also name as LevelList

class SongListScreen : public Screen {
public:
    ~SongListScreen() = default;

    SongListScreen();

    ScreenState Update() override;
private:
    std::shared_ptr<Button> m_RandomOrder;
    std::vector<std::shared_ptr<SongListItem>> m_Items;
    std::shared_ptr<EventObject> m_NowSelect = nullptr;
    std::shared_ptr<EventObject> m_SFXSelect = nullptr;
    int m_SelectedIndex = 0;
    std::vector<std::pair<Levels, float>> m_SongsOrder = {
        {Levels::Chronos, 74.3f},
        {Levels::MilkyWay, 0.0f},
        {Levels::CheatCodes, 0.0f}
    };

    bool play = false;
};

#endif //JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP