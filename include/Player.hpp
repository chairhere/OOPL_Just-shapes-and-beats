//
// Created by cheese on 2026/3/13.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYER_HPP
#define JUST_SHAPES_AND_BEATS_PLAYER_HPP
#include "glm/vec2.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"

class Player : public Util::GameObject {
public:
    Player() {
        m_Drawable = std::make_shared<Util::Image>("../Resources/Image/Player/Rectangle.png");
        m_Transform.scale = glm::vec2(0.3f, 0.3f);
    }

    [[nodiscard]] glm::vec2 GetPosition() {return m_Transform.translation;}

    [[nodiscard]] float GetRotation() {return m_Transform.rotation;}

    [[nodiscard]] glm::vec2 GetScale() {return m_Transform.scale;}

    void SetPosition(glm::vec2 new_position);

    void MovePosition(glm::vec2 movement);

    void SetRotation(float arc);

    void Moving();

    void Dash();

protected:
    int m_MaxHealth = 3;
    int m_Health = m_MaxHealth;
    int m_MaxResurrection = 3;
    int m_Resurrection_Times = m_MaxResurrection;
    glm::vec2 m_MovingDirection = glm::vec2(0.0f, 0.0f);
    bool m_Dashing = false;
    float m_DashTimeLeft = 0;
    bool m_DashCoolDown = false;

private:
};

#endif //JUST_SHAPES_AND_BEATS_PLAYER_HPP