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
    bool m_IsColliding = false;

    std::vector<float> m_LocalVertices;
    std::vector<float> m_WorldVertices;
    std::vector<float> m_WorldUVs;

public:

    Util::Transform m_Transform;
    SpawnEvent m_Event;

    std::function<void(Obstacle&, float)> customBehavior = nullptr;

    explicit Obstacle(const SpawnEvent& event, const std::vector<float> LocalVertices) : m_Event(event) {
        m_Transform.translation = event.StartPos;
        m_Transform.rotation = event.StartRot;
        m_Transform.scale = glm::vec2(20.0f, 20.0f);
        m_LocalVertices = LocalVertices;
        m_WorldUVs.reserve(m_LocalVertices.size());
        for (int i = 0; i < m_LocalVertices.size()/2; i++) {
            m_WorldUVs.push_back(0.25f);
            m_WorldUVs.push_back(0.5f);
        }

    }

    void UpdateStateByBeat(float currentBeat, glm::vec2 PlayerPos);

    void UpdateWorldVertices();

    bool CheckCollision(glm::vec2 PlayerPos) const;

    bool CheckCircleCollision(glm::vec2 PlayerPos) const;

    std::vector<float> GetWorldVertices(){return m_WorldVertices;}

    std::vector<float> GetWorldUVs(){return m_WorldUVs;}

    std::vector<float> GetLocalVertices(){return m_LocalVertices;}

    bool IsDead(){return m_IsDead;}

};

#endif //JUST_SHAPES_AND_BEATS_OBSTACLE_HPP