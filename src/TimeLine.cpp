//
// Created by jerry on 2026/3/12.
//
#include "TimeLine.hpp"

TimeLine::TimeLine(const std::string &path, float BPM){
    this->BPM = BPM;
    BGM_Player = std::make_unique<Util::BGM>(path);
    CurrentState = State::Load;
}

void TimeLine::Start() {
    CurrentState = State::Update;
    BGM_Player->Play();
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
    BGM_Player->Pause();

}

void TimeLine::Stop() {

}
