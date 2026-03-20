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
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        Dash();
    }
    if (m_Dashing) {
        m_MovingDirection *= 10;
        m_DashTimeLeft -= Util::Time::GetDeltaTimeMs();
        if (m_DashTimeLeft <= 0) {
            m_Dashing = false;
        }
    }else if (m_DashCoolDown) {  //not m_Dashing but m_DashCoolDown
        m_DashTimeLeft -= Util::Time::GetDeltaTimeMs();
        //0.4s cooldown time
        if (m_DashTimeLeft <= -400.0f and not Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
            m_DashCoolDown = false;
        }
    }
    MovePosition(m_MovingDirection);
    m_MovingDirection = glm::vec2(0.0f, 0.0f);
}

void Player::Dash() {
    if (not m_DashCoolDown) {
        m_Dashing = true;
        m_DashTimeLeft = 100.0f;
        m_DashCoolDown = true;
    }
}