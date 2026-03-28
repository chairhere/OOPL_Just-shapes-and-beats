//
// Created by cheese on 2026/3/28.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLIST_HPP
#define JUST_SHAPES_AND_BEATS_SONGLIST_HPP
#include <vector>

#include "SongData.hpp"
#include "SongsBPM.hpp"

class SongList {
public:
    inline static const std::vector<SongData> SongDatas = {
        SongData{"Chronos", "Danimal Cannon & Zef", SongsBPM::Chronos, "../Resources/Audio/Chronos.mp3"}
    };
};

#endif //JUST_SHAPES_AND_BEATS_SONGLIST_HPP