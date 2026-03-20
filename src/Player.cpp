//
// Created by cheese on 2026/3/13.
//

#include "Player.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"

void Player::SetPosition(glm::vec2 new_position) {
    m_Transform.translation = new_position;
}


void Player::MovePosition(glm::vec2 movement) {
    m_Transform.translation.x += movement.x;
    m_Transform.translation.y += movement.y;
}


void Player::SetRotation(float arc) {
    m_Transform.rotation = arc;
}

void Player::Moving() {
    if (Util::Input::IsKeyPressed(Util::Keycode::W) or Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        m_MovingDirection += glm::vec2(0.0f, 3.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S) or Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        m_MovingDirection += glm::vec2(0.0f, -3.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A) or Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        m_MovingDirection += glm::vec2(-3.0f, 0.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D) or Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        m_MovingDirection += glm::vec2(3.0f, 0.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        Dash();
    }
    if (m_Dashing) {
        m_MovingDirection *= 3;
        m_DashTimeLeft -= Util::Time::GetDeltaTimeMs();
        if (m_DashTimeLeft <= 0) {
            m_Dashing = false;
        }
    }
    MovePosition(m_MovingDirection);
    m_MovingDirection = glm::vec2(0.0f, 0.0f);
}

void Player::Dash() {
    m_Dashing = true;
    m_DashTimeLeft = 100.0f;
}