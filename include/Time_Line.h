//
// Created by jerry on 2026/3/12.
//

/**
 * Time_Line是用來播放歌曲與呼叫障礙產生器的主程式，歌曲的暫停播放與校正皆由此程式完成
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

    TimeLine(std::string &path, int BPM);
    ~TimeLine() = default;

    void Start();

    void Update();

    void Pause();

    void Stop();

private:
    Util::Time timer;
    std::unique_ptr<Util::BGM> BGM_Player;
    int BPM = 0;
    int Current_Beat = 0;
    Uint64 Start_Time = 0;
    Uint64 Current_Time = 0;
    State current_state = State::Load;


};

#endif //JUST_SHAPES_AND_BEATS_TIME_LINE_H