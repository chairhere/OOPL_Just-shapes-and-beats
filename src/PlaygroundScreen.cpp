//
// Created by cheese on 2026/3/27.
//

#include "../include/PlaygroundScreen.hpp"
PlaygroundScreen::PlaygroundScreen() {
    SDL_ShowCursor(SDL_DISABLE);
}

ScreenState PlaygroundScreen::Update() {
    return ScreenState::Playground;
}
