//
// Created by cheese on 2026/4/17.
//

#ifndef JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#define JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#include "EventObject.hpp"

class DiscreteSlider : public EventObject {
public:
    //20格 一格0.05
    DiscreteSlider(int total_steps, float startX, float startY, float space = 5.0f);

    void Update() override;
};

#endif //JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP