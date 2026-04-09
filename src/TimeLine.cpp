//
// Created by jerry on 2026/3/12.
//
#include "TimeLine.hpp"

TimeLine::TimeLine(const std::string &path, float BPM){
    this->BPM = BPM;
    BGMPlayer = std::make_unique<Util::BGM>(path);
    StartTime = 0.0f;
}

void TimeLine::Start() {
    if (!m_Isplaying && CurrentTime == 0.0f) {
        m_Isplaying = true;
        BGMPlayer->SetVolume(3);
        BGMPlayer->Play();
        StartTime = Util::Time::GetElapsedTimeMs();
    }
}

void TimeLine::Update() {
    if (m_Isplaying) {
        CurrentTime = Util::Time::GetElapsedTimeMs() - StartTime - PauseDuration;
        CurrentBeat = ((CurrentTime / 1000.0f)* (BPM / 60));
    }
}

void TimeLine::Resume() {
    if (!m_Isplaying && CurrentTime != 0.0f) {
        m_Isplaying = true;
        PauseDuration += Util::Time::GetElapsedTimeMs() - PauseTime;
        BGMPlayer->Resume();
    }
}

void TimeLine::Pause() {
    if (m_Isplaying) {
        m_Isplaying = false;
        PauseTime = Util::Time::GetElapsedTimeMs();
        BGMPlayer->Pause();
    }
}

void TimeLine::Stop() {
    PauseDuration = 0.0f;
    CurrentTime = 0.0f;
    m_Isplaying = false;
    BGMPlayer.reset();
}

void TimeLine::ChangeBGM(const std::string &path) {
    if (m_Isplaying) {
        m_Isplaying = false;
        BGMPlayer->Pause();
        BGMPlayer.reset();
    }
    StartTime = 0.0f;
    CurrentTime = 0.0f;
    BGMPlayer->LoadMedia(path);
}

float TimeLine::GetBeats() const {
    return CurrentBeat;
}

void TimeLine::SetVolume(float volume) {
    BGMPlayer->SetVolume(volume);
}
