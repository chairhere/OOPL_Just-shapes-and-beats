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
#include "Spawner/SpawnEvent.hpp"

class Obstacle{
private:
    bool m_IsActive = false;
    bool m_IsDead = false; // 標記是否已經超過 endBeat，準備被銷毀
    bool m_Collidable = false;
    bool m_IsShaked = true;

    float m_LastBeat;

    std::vector<float> m_LocalVertices;
    std::vector<float> m_WorldVertices;
    std::vector<float> m_WorldUVs;

public:

    Util::Transform m_Transform;
    SpawnEvent m_Event{};

    std::vector<float> m_TempUVs = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f};

    bool m_IsColliding = false;

    std::function<void(Obstacle&, float, glm::vec2)> customBehavior = nullptr;

    explicit Obstacle();

    void Spawn(const SpawnEvent& event, const std::vector<float>& LocalVertices);

    void UpdateStateByBeat(float currentBeat, glm::vec2 PlayerPos);

    void UpdateWorldVertices();

    void SetUvs(const std::vector<float>& Uvs);

    bool CheckCollision(glm::vec2 PlayerPos) const;

    bool CheckOtherCollision(glm::vec2 PlayerPos) const;

    bool CheckCircleCollision(glm::vec2 PlayerPos) const;

    const std::vector<float>& GetWorldVertices() const {return m_WorldVertices;}

    const std::vector<float>& GetWorldUVs() const {return m_WorldUVs;}

    const std::vector<float>& GetLocalVertices() const {return m_LocalVertices;}

    float GetLastBeat(){return m_LastBeat;}

    bool IsDead(){return m_IsDead;}

    bool IsActive(){return m_IsActive;}

    bool IsShaked(){return m_IsShaked;}

    void HasShaked(){m_IsShaked = true;}

    void TurnOnCollidable(){m_Collidable = true;}

    void TurnOffCollidable(){m_Collidable = false;}

    void ClearEvent();


};

#endif //JUST_SHAPES_AND_BEATS_OBSTACLE_HPP