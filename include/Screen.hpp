//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_SCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SCREEN_HPP
#include "Util/AssetStore.hpp"
#include "Util/Renderer.hpp"

class Screen {
public:
    virtual ~Screen() = default;

    virtual void Update() = 0;

protected:
    Util::Renderer m_Renderer;
};

#endif //JUST_SHAPES_AND_BEATS_SCREEN_HPP