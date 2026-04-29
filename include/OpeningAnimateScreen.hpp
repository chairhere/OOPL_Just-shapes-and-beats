//
// Created by LAB1223 on 2026/4/24.
//

#ifndef JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
#define JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP

#include "Screen.hpp"
#include "ScreenState.hpp"
#include "FadeLayer.hpp"
#include "CustomColorShape.hpp"

class PlaygroundScreen : public Screen {
public:
    ~PlaygroundScreen() override = default;

    explicit PlaygroundScreen(Levels level);

    ScreenState Update() override;
};

#endif //JUST_SHAPES_AND_BEATS_OPENINGANIMATESCREEN_HPP
