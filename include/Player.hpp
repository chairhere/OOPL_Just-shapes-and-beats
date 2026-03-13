//
// Created by cheese on 2026/3/13.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYER_HPP
#define JUST_SHAPES_AND_BEATS_PLAYER_HPP
#include "glm/vec2.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Player : public Util::GameObject {
public:
    Player() {
        m_Drawable =std::make_shared<Util::Image>("../Resources/Image/Player/player1.png");
        m_Transform.scale = glm::vec2(50.0f, 50.0f);
    };

    [[nodiscard]] glm::vec2 GetPosition() {return m_Transform.translation;}

    [[nodiscard]] float GetRotation() {return m_Transform.rotation;}

    [[nodiscard]] glm::vec2 GetScale() {return m_Transform.scale;}

    void SetPosition(glm::vec2 new_position);

    void MovePosition(glm::vec2 movement);

    void SetRotation(float arc);

    void Moving();

    void Dash();

protected:
    int Max_Health = 3;
    int Health = Max_Health;
    glm::vec2 Moving_Direction = glm::vec2(0.0f, 0.0f);
    bool Dashing = false;
    float Dash_Time_Left = 0;

private:
};

#endif //JUST_SHAPES_AND_BEATS_PLAYER_HPP