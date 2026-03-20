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

    // 每個畫面都必須實作自己的 Update 邏輯
    virtual void Update() = 0;

protected:
    // 每個畫面都有專屬的渲染器，用來管理跟繪製自己的物件
    Util::Renderer m_Renderer;
};

#endif //JUST_SHAPES_AND_BEATS_SCREEN_HPP