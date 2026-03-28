//
// Created by cheese on 2026/3/28.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGDATA_HPP
#define JUST_SHAPES_AND_BEATS_SONGDATA_HPP
#include <string>

#include "SongsBPM.hpp"

struct SongData {
public:
    std::string Title;
    std::string Composer;
    SongsBPM BPM;
    std::string AudioPath;
};

#endif //JUST_SHAPES_AND_BEATS_SONGDATA_HPP