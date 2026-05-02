//
// Created by cheese on 2026/4/5.
//

#ifndef JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP
#define JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP
#include <memory>
#include <random>

#include "Tool/Levels.hpp"
#include "Screen/Screen.hpp"
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

class MusicPlayerManager {
public:
    //  對音樂操作時，請務必搭配清單操作一起食用，效果更佳
    static MusicPlayerManager& Setting() {
        static MusicPlayerManager my_self;
        return my_self;
    }

    //  音樂操作
    void Pause();  //暫停
    void Play();  //播放
    void Stop();  //停止
    void ShunDown();  //關閉撥放器

    void Next();  //下一首
    void PlayAt(float beats);  //中途播放(拍)
    void ReverseAt(float beats);  //倒放(拍)
    void Switch(Levels music);  //切換目前歌曲
    void PlayAtTime(float sec);  //中途播放(秒)

    void SetSFXVolume(float volume);  //調整音效音量 0.0~1.0
    void SetBGMVolume(float volume);  //調整音樂音量 0.0~1.0
    void SetSpeed(float speed);  //調整播放速度
    void InfLoop(bool inf);  //調整循環撥放

    float GetBeats();  //取得播放進度
    Levels GetCurrentLevel();  //取得當前歌曲(關卡)
    float GetTotalBeats();  //取得總節奏長度
    float GetTotalLength();  //取得總時長
    bool IsPause(); //是否暫停

    //  清單操作
    void AddMusic(Levels music);  //新增音樂
    void RemoveMusic(Levels music);  //移出音樂
    void CleanList();  //清除待放清單

    void RandomTheList();  //打亂清單

    bool IsEmpty();  //確認音樂撥放完畢

    //  音效操作
    enum Effect {
        BtClick,
        BtSelect,
        Cancel,
        Choose,
        PlrDie,
        PlrHit,
        PlrHit1,
        PlrHit2,
        Return,
        PlrRevive
    };
    void PlayEffect(Effect effect);

private:
    MusicPlayerManager();

    std::vector<Levels> m_MusicList;  //音樂清單

    SoLoud::Soloud m_MusicPlayer;  //引擎本體
    std::unordered_map<Effect, SoLoud::Wav> m_SFXLibrary;
    SoLoud::Wav m_BGM;  //長音樂物件
    SoLoud::handle m_BGMHandler;  //音樂處理ID
    float BGMVolume = 0.3f;
    float SFXVolume = 0.3f;
    bool reverse = false;
    float currentBeats = 0.0f;

    std::random_device rd;  //隨機種子
    std::mt19937 g = std::mt19937(rd());  //取亂數
    std::bernoulli_distribution dist = std::bernoulli_distribution(0.5);  //0 or 1, 50%機率
};

#endif //JUST_SHAPES_AND_BEATS_MUSICPLAYERMANAGER_HPP