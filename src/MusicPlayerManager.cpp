//
// Created by cheese on 2026/4/5.
//

#include "MusicPlayerManager.hpp"

#include <random>

#include "SongList.hpp"

MusicPlayerManager::MusicPlayerManager() {
    m_MusicPlayer.init();
    m_SFXLibrary[BtClick].load("../Resources/Audio/Effect/ButtonClick.ogg");
    m_SFXLibrary[BtSelect].load("../Resources/Audio/Effect/ButtonSelect.ogg");
    m_SFXLibrary[Cancel].load("../Resources/Audio/Effect/Cancel.ogg");
    m_SFXLibrary[Choose].load("../Resources/Audio/Effect/Choose.ogg");
    m_SFXLibrary[PlrDie].load("../Resources/Audio/Effect/Die.ogg");
    m_SFXLibrary[PlrHit1].load("../Resources/Audio/Effect/Hit1.ogg");
    m_SFXLibrary[PlrHit2].load("../Resources/Audio/Effect/Hit2.ogg");
    m_SFXLibrary[Return].load("../Resources/Audio/Effect/Return.ogg");
    m_SFXLibrary[PlrRevive].load("../Resources/Audio/Effect/Revive.ogg");
    SetBGMVolume(0.2f);
    SetSFXVolume(0.2f);
}

void MusicPlayerManager::Pause() {
    if (IsEmpty()) return;
    m_MusicPlayer.setPause(m_BGMHandler, true);
}

void MusicPlayerManager::Play() {
    if (IsEmpty()) return;
    if (m_MusicPlayer.isValidVoiceHandle(m_BGMHandler) && m_MusicPlayer.getPause(m_BGMHandler)) {
        m_MusicPlayer.setPause(m_BGMHandler, false);
    }else {  //並非暫停處理
        m_BGMHandler = m_MusicPlayer.play(m_BGM);
    }
}

void MusicPlayerManager::Stop() {
    if (IsEmpty()) return;
    m_MusicPlayer.stop(m_BGMHandler);
}

void MusicPlayerManager::ShunDown() {
    CleanList();
    m_MusicPlayer.deinit();
}

void MusicPlayerManager::Next() {
    if (IsEmpty()) return;

    m_MusicList.erase(m_MusicList.begin());
    if (m_MusicList.empty()) {
        Stop();
    }else {
        Switch(m_MusicList[0]);
    }
}

void MusicPlayerManager::PlayAt(float beats) {
    if (IsEmpty()) return;

    SongData data = SongList::GetSongByName(m_MusicList[0]);
    float times = beats * 60.0f / static_cast<float>(data.BPM);
    m_MusicPlayer.seek(m_BGMHandler, times);
}

void MusicPlayerManager::ReverseAt(float beats) {
    if (IsEmpty()) return;

    SongData data = SongList::GetSongByName(m_MusicList[0]);
    if (data.ReverseAudioPath.empty()) return;

    Stop();
    m_BGM.load(data.ReverseAudioPath.c_str());
    m_BGMHandler = m_MusicPlayer.play(m_BGM);

    float totalLen = (float)m_BGM.getLength();
    float times = beats * 60.0f / static_cast<float>(data.BPM);
    float reverseStartTime = totalLen - times;
    m_MusicPlayer.seek(m_BGMHandler, reverseStartTime);
}

void MusicPlayerManager::Switch(Levels music) {
    if (IsEmpty()) {
        m_MusicList.push_back(music);
    }else {
        if (music == m_MusicList[0]) return;
        Stop();
        m_MusicList[0] = music;
    }
    SongData data = SongList::GetSongByName(music);
    m_BGM.load(data.AudioPath.c_str());
    Play();
}

void MusicPlayerManager::SetSFXVolume(float volume) {
    // 0.0 ~ 1.0
    for (auto& pair : m_SFXLibrary) {
        pair.second.setVolume(volume);
    }
}

void MusicPlayerManager::SetBGMVolume(float volume) {
    // 0.0 ~ 1.0
    m_BGM.setVolume(volume);
}

void MusicPlayerManager::SetSpeed(float speed) {
    //1.0為正常 >1.0為加速 <1.0為減速
    m_MusicPlayer.setRelativePlaySpeed(m_BGMHandler, speed);
}

void MusicPlayerManager::InfLoop(bool inf) {
    m_BGM.setLooping(inf);
}

float MusicPlayerManager::GetBeats() {
    if (IsEmpty()) throw std::invalid_argument("List is empty");

    SongData data = SongList::GetSongByName(m_MusicList[0]);
    return static_cast<float>(m_MusicPlayer.getStreamTime(m_BGMHandler)) / 60.0f * static_cast<float>(data.BPM);
}

Levels MusicPlayerManager::GetCurrentLevel() {
    if (IsEmpty()) throw std::invalid_argument("No level running");

    return m_MusicList[0];
}


void MusicPlayerManager::AddMusic(Levels music) {
    auto it = std::find(m_MusicList.begin(), m_MusicList.end(), music);
    if (it == m_MusicList.end()) {
        m_MusicList.push_back(music);
    }
}

void MusicPlayerManager::RemoveMusic(Levels music) {
    auto it = std::find(m_MusicList.begin(), m_MusicList.end(), music);
    if (it != m_MusicList.end()) {
        m_MusicList.erase(it);
    }
}

void MusicPlayerManager::CleanList() {
    Stop();
    m_MusicList.clear();
}

void MusicPlayerManager::RandomTheList() {
    std::shuffle(m_MusicList.begin(), m_MusicList.end(), g);
}

bool MusicPlayerManager::IsEmpty() {
    return m_MusicList.empty();
}

void MusicPlayerManager::PlayEffect(Effect effect) {
    switch (effect) {
        case PlrHit:
            if (dist(g)) {
                m_MusicPlayer.play(m_SFXLibrary[PlrHit1]);
            }else {
                m_MusicPlayer.play(m_SFXLibrary[PlrHit2]);
            }
        default:
            m_MusicPlayer.play(m_SFXLibrary[effect]);
    }
}
