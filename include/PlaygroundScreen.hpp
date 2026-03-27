//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP
#include "Screen.hpp"
#include "ScreenState.hpp"

class PlaygroundScreen : public Screen {
public:
    ~PlaygroundScreen() = default;

    PlaygroundScreen();

    ScreenState Update() override;
private:
};

#endif //JUST_SHAPES_AND_BEATS_PLAYGROUNDSCREEN_HPP