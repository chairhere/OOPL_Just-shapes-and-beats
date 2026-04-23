//
// Created by cheese on 2026/3/13.
//

#include "Player.hpp"

#include "config.hpp"
#include "MusicPlayerManager.hpp"


void Player::SetPosition(glm::vec2 new_position) {
    m_Transform.translation = new_position;
}


void Player::MovePosition(glm::vec2 movement) {
    //邊界偵測
    m_Transform.translation.x += movement.x;
    m_Transform.translation.y += movement.y;

    // 2. 取得玩家自身的半寬與半高
    // 這是為了防止玩家的中心點卡在螢幕邊緣，導致「半個身體掉到畫面外」
    glm::vec2 size = m_Drawable->GetSize() * m_Transform.scale;
    float halfWidth = size.x / 2.0f;
    float halfHeight = size.y / 2.0f;

    // 3. 計算螢幕的四大邊界 (原點在正中央)
    float minX = -768.0f + halfWidth;  // 左邊界
    float maxX = 768.0 - halfWidth;  // 右邊界
    float minY = -433.0f + halfHeight; // 下邊界 (根據您的 Y 軸方向可能需要微調)
    float maxY = 433.0f - halfHeight; // 上邊界

    // 4. 【核心邏輯】將玩家的 X 與 Y 座標強制限制在邊界內
    m_Transform.translation.x = std::clamp(m_Transform.translation.x, minX, maxX);
    m_Transform.translation.y = std::clamp(m_Transform.translation.y, minY, maxY);
}


void Player::SetRotation(float arc) {
    m_Transform.rotation = arc;
}

bool Player::Moving() {
    if (not m_Stun) {
        if (Util::Input::IsKeyPressed(Util::Keycode::W) or Util::Input::IsKeyPressed(Util::Keycode::UP)) {
            m_MovingDirection += glm::vec2(0.0f, m_Speed);
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::S) or Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
            m_MovingDirection += glm::vec2(0.0f, -m_Speed);
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::A) or Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
            m_MovingDirection += glm::vec2(-m_Speed, 0.0f);
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::D) or Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
            m_MovingDirection += glm::vec2(m_Speed, 0.0f);
        }
        if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
            Dash();
        }
    }

    if (m_Dashing) {
        m_MovingDirection *= 5;
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
        if (m_KnockBackDirection == glm::vec2(0.0f, 0.0f)) {
            if (m_MovingDirection == glm::vec2(0.0f, 0.0f)) {
                m_MovingDirection = glm::vec2(m_Speed, 0.0f);
            }
            m_KnockBackDirection = m_MovingDirection * -3.0f;
        }
        if (m_Stun)
            m_MovingDirection = m_KnockBackDirection;
        if (m_InvincibleTimeLeft <= 100)
            m_Stun = false;
        if (m_InvincibleTimeLeft <= 0) {
            m_KnockBack = false;
            m_Invincible = false;
            m_KnockBackDirection = glm::vec2(0.0f, 0.0f);
        }
    }
    if (m_NoDamage) {
        m_NoDamageTimeLeft -= Util::Time::GetDeltaTimeMs();
        if (m_NoDamageTimeLeft <= 0) {
            m_NoDamage = false;
        }
    }
    m_Transform.translation -= m_LastOffset;
    m_LastOffset = glm::vec2(0.0f, 0.0f);
    MovePosition(m_MovingDirection);
    m_MovingDirection = glm::vec2(0.0f, 0.0f);
    return m_Health == 0;
}

void Player::Dash() {
    if (not m_DashCoolDown) {
        if (m_MovingDirection == glm::vec2(0.0f, 0.0f)) {
            m_MovingDirection = glm::vec2(3.0f, 0.0f);
        }
        m_Dashing = true;
        m_Invincible = true;
        m_DashTimeLeft = 100.0f;
        m_DashCoolDown = true;
    }
}

void Player::Hit() {
    if (m_Invincible || m_Health <= 0) return;
    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrHit);
    if (not m_NoDamage && m_Health > 0)
        m_Health -= 1;
    m_Stun = true;
    m_KnockBack = true;
    m_Invincible = true;
    m_InvincibleTimeLeft = 500.0f;
}

void Player::Shake(glm::vec2 movement) {
    if (movement == glm::vec2(0.0f, 0.0f)) {
        return;
    }
    m_LastOffset = movement;
    m_Transform.translation += movement;
}

void Player::Die() {
    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrDie);
    m_Health = 0;
    this->SetVisible(false);
}

void Player::Revive() {
    MusicPlayerManager::Setting().PlayEffect(MusicPlayerManager::PlrRevive);
    m_Health = m_MaxHealth;
    m_Transform.translation = glm::vec2(-500.0f, 0.0f);
    this->SetVisible(true);
    m_NoDamage = true;
    m_NoDamageTimeLeft = 1000.0f;
}
