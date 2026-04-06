//
// Created by cheese on 2026/4/5.
//

#ifndef JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP
#define JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP
#include <memory>

#include "Screen.hpp"

class MusicPlayerManager {
public:
    static MusicPlayerManager& Get() {
        static MusicPlayerManager my_self;
        return my_self;
    }

private:
    MusicPlayerManager() = default;
    std::shared_ptr<Screen> LastOperScreen = nullptr;

};

#endif //JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP