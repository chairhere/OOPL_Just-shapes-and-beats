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

    TimeLine(const std::string &path, float BPM);
    ~TimeLine() = default;

    void Start();

    void Update();

    void Resume();

    void Pause();

    void Stop();

    void ChangeBGM(const std::string &path);

    float GetBeats() const;

    [[nodiscard]] bool IsPlaying() const{return m_Isplaying;}

    void SetVolume(float volume);


private:
    Util::Time Timer;
    std::unique_ptr<Util::BGM> BGMPlayer;

    float BPM = 0.0f;
    float CurrentBeat = 0.0f;
    float StartTime = 0.0f;
    float CurrentTime = 0.0f;
    float PauseTime = 0.0f;
    float PauseDuration = 0.0f;

    bool m_Isplaying = false;


};

#endif //JUST_SHAPES_AND_BEATS_TIME_LINE_H