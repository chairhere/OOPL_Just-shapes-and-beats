//
// Created by cheese on 2026/3/28.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLIST_HPP
#define JUST_SHAPES_AND_BEATS_SONGLIST_HPP
#include <unordered_map>
#include <vector>

#include "Levels.hpp"
#include "SongData.hpp"
#include "SongsBPM.hpp"
#include "Util/Logger.hpp"

class SongList {
public:
    static const std::unordered_map<Levels, SongData>& GetAllSong() {
        static const std::unordered_map<Levels, SongData> s_Songs = {
            {Levels::Chronos, SongData{"Chronos", "Danimal Cannon & Zef", SongsBPM::Chronos, "../Resources/Audio/Chronos.mp3"}}
        };
        return s_Songs;
    };

    static const SongData& GetSongByName(const Levels songName) {
        const auto& songs = GetAllSong();

        if (songs.find(songName) != songs.end()) {
            return songs.at(songName);
        }else {
            LOG_ERROR("Song {} NOT found", to_string(songName));
            throw std::invalid_argument("Song not found");
        }
    }
};

#endif //JUST_SHAPES_AND_BEATS_SONGLIST_HPP