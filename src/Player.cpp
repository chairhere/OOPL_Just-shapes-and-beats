//
// Created by cheese on 2026/3/13.
//

#include "Player.hpp"

#include "MusicPlayerManager.hpp"


void Player::SetPosition(glm::vec2 new_position) {
    m_Transform.translation = new_position;
}


void Player::MovePosition(glm::vec2 movement) {
    float predict_x = m_Transform.translation.x;
    float predict_y = m_Transform.translation.y;
    //邊界偵測
    m_Transform.translation.x += movement.x;
    m_Transform.translation.y += movement.y;
}


void Player::SetRotation(float arc) {
    m_Transform.rotation = arc;
}

bool Player::Moving() {
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
            m_Invincible = false;
        }
    }else if (m_DashCoolDown) {  //not m_Dashing but m_DashCoolDown
        m_DashTimeLeft -= Util::Time::GetDeltaTimeMs();
        //0.4s cooldown time
        if (m_DashTimeLeft <= -400.0f and not Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
            m_DashCoolDown = false;
        }
    }
    if (m_KnockBack) {
        m_InvincibleTimeLeft -= Util::Time::GetDeltaTimeMs();
        if (m_MovingDirection == glm::vec2(0.0f, 0.0f)) {
            m_MovingDirection = glm::vec2(-30.0, 0.0f);
        }else {
            m_MovingDirection *= -10.0f;
        }
        if (m_InvincibleTimeLeft <= 0) {
            m_KnockBack = false;
            m_Invincible = false;
        }
    }
    MovePosition(m_MovingDirection);
    m_MovingDirection = glm::vec2(0.0f, 0.0f);
    return m_Health == 0;
}

void Player::Dash() {
    if (not m_DashCoolDown) {
        m_Dashing = true;
        m_Invincible = true;
        m_DashTimeLeft = 100.0f;
        m_DashCoolDown = true;
    }
}

void Player::Hit() {
    if (m_Invincible) return;
    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrHit);
    m_Health -= 1;
    m_Invincible = true;
    m_InvincibleTimeLeft = 1000.0f;
    m_KnockBack = true;
}
