//
// Created by jerry on 2026/3/12.
//
#include "TimeLine.hpp"

TimeLine::TimeLine(const std::string &path, float BPM){
    this->BPM = BPM;
    BGMPlayer = std::make_unique<Util::BGM>(path);
    CurrentState = State::Load;
}

void TimeLine::Start() {
    CurrentState = State::Update;
    BGMPlayer->SetVolume(3);
    BGMPlayer->Play();
    StartTime = SDL_GetPerformanceCounter();
}

void TimeLine::Update() {
    if (CurrentState == State::Update) {
        CurrentTime = static_cast<float>(SDL_GetPerformanceCounter() - StartTime) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f;
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
