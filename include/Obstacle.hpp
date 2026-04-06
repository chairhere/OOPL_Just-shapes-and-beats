//
// Created by jerry on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_OBSTACLE_HPP
#define JUST_SHAPES_AND_BEATS_OBSTACLE_HPP

#include "Util/GameObject.hpp"
#include "SpawnEvent.hpp"
#include <algorithm>


class Obstacle : public Util::GameObject {
public:
    SpawnEvent m_Event;
    bool m_IsDead = false; // 標記是否已經超過 endBeat，準備被銷毀


    explicit Obstacle(const SpawnEvent& event) : m_Event(event) {
        m_Transform.translation = event.StartPos;
        m_Transform.rotation = event.StartRot;
        m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void UpdateStateByBeat(float currentBeat);
};

#endif //JUST_SHAPES_AND_BEATS_OBSTACLE_HPP