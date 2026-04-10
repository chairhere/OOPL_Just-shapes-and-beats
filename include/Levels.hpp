//
// Created by cheese on 2026/3/27.
//

#ifndef JUST_SHAPES_AND_BEATS_LEVELS_HPP
#define JUST_SHAPES_AND_BEATS_LEVELS_HPP

enum class Levels {
    Chronos,
    MilkyWay,
    CheatCodes/*,
    Opening,
    Barracuda,
    FinalBoss,
    FTW,
    HYPE,
    NewGame,
    OnceAgain,
    Rainbow,
    SugarRush,
    TillItsOver,
    TryThis*/
};

constexpr std::string_view to_string(const Levels L) {
    switch (L) {
        case Levels::Chronos:
            return "Chronos";
        case Levels::MilkyWay:
            return "Milky Way";
        case Levels::CheatCodes:
            return "Cheat Codes";
    }
    return "Unknown*";
}

#endif //JUST_SHAPES_AND_BEATS_LEVELS_HPP