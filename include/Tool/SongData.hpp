//
// Created by cheese on 2026/3/28.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGDATA_HPP
#define JUST_SHAPES_AND_BEATS_SONGDATA_HPP
#include <string>

#include "Tool/SongsBPM.hpp"
#include "Tool/Levels.hpp"

struct SongData {
public:
    Levels Level;
    std::string Title;
    std::string Composer;
    SongsBPM BPM;
    std::string AudioPath;
    std::string ReverseAudioPath;
    std::string BeatMap;
};

#endif //JUST_SHAPES_AND_BEATS_SONGDATA_HPP