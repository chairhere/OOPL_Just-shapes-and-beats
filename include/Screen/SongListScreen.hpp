//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP

#include "EventObject/Button.hpp"
#include "Screen/Screen.hpp"
#include "EventObject/SongListItem.hpp"

//this also name as LevelList

class SongListScreen : public Screen {
public:
    ~SongListScreen() = default;

    SongListScreen();

    ScreenState Update() override;
private:
    // std::shared_ptr<Button> m_RandomOrder;
    std::vector<std::shared_ptr<SongListItem>> m_Items;
    std::shared_ptr<EventObject> m_NowSelect = nullptr;
    std::shared_ptr<EventObject> m_SFXSelect = nullptr;
    int m_SelectedIndex = 0;
    std::vector<Levels> m_SongsOrder = {
        Levels::Chronos,
        Levels::MilkyWay
        // Levels::CheatCodes
    };
    std::shared_ptr<ImageObject> m_Hint;

    bool play = false;
};

#endif //JUST_SHAPES_AND_BEATS_SONGLISTSCREEN_HPP