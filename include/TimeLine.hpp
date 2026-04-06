//
// Created by jerry on 2026/3/12.
//

/**
 * Time_Line是用來播放歌曲的主程式，歌曲的暫停播放與校正皆由此程式完成
 */
#ifndef JUST_SHAPES_AND_BEATS_TIME_LINE_H
#define JUST_SHAPES_AND_BEATS_TIME_LINE_H

#include "Util/Time.hpp"
#include "Util/BGM.hpp"

class TimeLine{
public:
    enum class State {
        Load,
        Start,
        Update,
        Pause,
        Stop
    };

    TimeLine(const std::string &path, float BPM);
    ~TimeLine() = default;

    void Start();

    void Update();

    void Pause();

    void Stop();

    void ChangeBGM(const std::string &path);

    float GetBeats() const;

    [[nodiscard]] State GetState() const {return CurrentState;}

private:
    Util::Time Timer;
    std::unique_ptr<Util::BGM> BGMPlayer;

    float BPM = 0.0f;
    float CurrentBeat = 0.0f;
    float StartTime = 0;
    float CurrentTime = 0;

    State CurrentState = State::Load;


};

#endif //JUST_SHAPES_AND_BEATS_TIME_LINE_H