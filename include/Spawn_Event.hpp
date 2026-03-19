//
// Created by jerry on 2026/3/19.
//

#ifndef JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP
#define JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP

#include <glm/glm.hpp>
#include <string>

// 紀錄譜面中一個障礙物的生成與行為資訊 [4]
struct SpawnEvent {
    float startBeat;       // 出現的節拍點
    float endBeat;         // 消失的節拍點

    glm::vec2 startPos;    // 初始位置
    glm::vec2 endPos;      // 結束位置 (若與初始相同代表不移動)

    float startRot;        // 初始旋轉角度 (弧度) [1]
    float endRot;          // 結束旋轉角度 (弧度)

    std::string shapeType; // 障礙物形狀 (例如 "Square", "Laser")
};

#endif //JUST_SHAPES_AND_BEATS_SPAWN_EVENT_HPP