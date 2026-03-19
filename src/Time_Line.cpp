//
// Created by jerry on 2026/3/12.
//
#include "Time_Line.hpp"

TimeLine::TimeLine(std::string &path, float BPM){
    this->BPM = BPM;
    BGM_Player = std::make_unique<Util::BGM>(path);
    Current_State = State::Load;
}

void TimeLine::Start() {
    Current_State = State::Update;
    BGM_Player->Play();
    Start_Time = SDL_GetPerformanceCounter();
}

void TimeLine::Update() {
    if (Current_State == State::Update) {
        Current_Time = static_cast<float>(SDL_GetPerformanceCounter() - Start_Time) / static_cast<float>(SDL_GetPerformanceFrequency()) * 1000.0f;
        Current_Beat = (Current_Time * (BPM / 60));
        Current_State = State::Update;
    }
}

void TimeLine::Pause() {
    Current_State = State::Pause;
    BGM_Player->Pause();

}
