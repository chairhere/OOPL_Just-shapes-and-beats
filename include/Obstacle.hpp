//
// Created by jerry on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_OBSTACLE_HPP
#define JUST_SHAPES_AND_BEATS_OBSTACLE_HPP

#include <glm/glm.hpp>
#include <algorithm> // 為了使用 std::clamp
#include "Util/Transform.hpp"
#include "Util/Color.hpp"
#include <functional>
#include "SpawnEvent.hpp"

class Obstacle{
private:
    bool m_IsDead = false; // 標記是否已經超過 endBeat，準備被銷毀
    Util::Transform m_Transform;
    std::vector<float> m_LocalVertices;
    std::vector<float> m_WorldVertices;
    std::vector<float> m_WorldUVs;
    std::function<void(Obstacle&, float)> customBehavior = nullptr;
    SpawnEvent m_Event;
public:

    explicit Obstacle(const SpawnEvent& event) : m_Event(event) {
        m_Transform.translation = event.StartPos;
        m_Transform.rotation = event.StartRot;
        m_Transform.scale = glm::vec2(1.0f, 1.0f);
    }

    void UpdateStateByBeat(float currentBeat);

    void UpdateWorldVertices();
};

#endif //JUST_SHAPES_AND_BEATS_OBSTACLE_HPP