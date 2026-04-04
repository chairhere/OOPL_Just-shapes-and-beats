//
// Created by jerry on 2026/3/12.
//
#include "TimeLine.hpp"

TimeLine::TimeLine(const std::string &path, float BPM){
    this->BPM = BPM;
    BGMPlayer = std::make_unique<Util::BGM>(path);
    CurrentState = State::Load;
    StartTime = 0.0f;
}

void TimeLine::Start() {
    CurrentState = State::Update;
    BGMPlayer->SetVolume(3);
    BGMPlayer->Play();
}

void TimeLine::Update() {
    if (CurrentState == State::Update) {
        CurrentTime += Util::Time::GetDeltaTimeMs();
        CurrentBeat = (CurrentTime * (BPM / 60));
        CurrentState = State::Update;
    }
}

void TimeLine::Pause() {
    CurrentState = State::Pause;
    BGMPlayer->Pause();

}

void TimeLine::Stop() {
    CurrentState = State::Stop;
    BGMPlayer.reset();
}

void TimeLine::ChangeBGM(const std::string &path) {
    BGMPlayer->LoadMedia(path);
}

float TimeLine::GetBeats() {
    return CurrentBeat;
}
