//
// Created by jerry on 2026/3/12.
//
#include "Time_Line.h"

TimeLine::TimeLine(std::string &path, int BPM){
    this->BPM = BPM;
    BGM_Player = std::make_unique<Util::BGM>(path);
    current_state = State::Load;
}

void TimeLine::Start() {
    current_state = State::Update;
    BGM_Player->Play();
    Start_Time = SDL_GetPerformanceCounter();
}
