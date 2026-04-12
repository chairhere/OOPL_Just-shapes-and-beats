//
// Created by jerry on 2026/3/19.
//

#ifndef JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP
#define JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP

#include <glm/glm.hpp>
#include <string>
#include <variant>
#include "BulletType.hpp"

//using BulletSpec = std::variant<TrackingData, WaveData, LinearData>;
// 紀錄譜面中一個障礙物的生成與行為資訊 [4]



struct SpawnEvent {
    float StartBeat;       // 出現的節拍點
    float EndBeat;         // 消失的節拍點

    glm::vec2 StartPos;    // 初始位置
    glm::vec2 EndPos;      // 結束位置 (若與初始相同代表不移動)

    float StartRot;        // 初始旋轉角度 (弧度) [1]
    float EndRot;          // 結束旋轉角度 (弧度)

    BulletType ShapeType; // 障礙物形狀 (例如 "Square", "Laser")

    struct SpecialEvent {
        float SpawnBeat;
        float PauseBeat;
        float AngularVelocity;//角速度

        glm::vec2 PausePos;
        glm::vec2 Velocity;

    } SpecialData;
};

struct RR {
    SpawnEvent SpawnEvent;

    glm::vec2 Velocity;
    float AngularVelocity;
};


#endif //JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP