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
    if (not m_Stun) {  //非硬直則移動
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
        Squash();
        Turn();
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
        m_Blink ^= true;
        if (m_Stun)
            m_MovingDirection = m_KnockBackDirection;
        if (m_InvincibleTimeLeft <= 500)
            m_Stun = false;
        if (m_InvincibleTimeLeft <= 0) {
            m_KnockBack = false;
            m_Invincible = false;
            m_Blink = false;
            m_KnockBackDirection = glm::vec2(0.0f, 0.0f);
        }
        if (m_Blink) {
            if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
                imageDrawable->SetImage(m_BlinkImagePath);
            }
        }else {
            if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
                imageDrawable->SetImage(m_NowImagePath);
            }
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
    m_InvincibleTimeLeft = 1000.0f;
    ChangeImage();
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
    ChangeImage();
}

void Player::ChangeImage() {
    if (m_MaxHealth == 3) {
        switch (m_Health) {
            case 3:
                m_NowImagePath = "../Resources/Image/Player/Rectangle.png";
                break;
            case 2:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-2.png";
                break;
            case 1:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-4.png";
                break;
            default:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-6.png";
                break;
        }
    }else if (m_MaxHealth == 6) {
        switch (m_Health) {
            case 6:
                m_NowImagePath = "../Resources/Image/Player/Rectangle.png";
                break;
            case 5:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-1.png";
                break;
            case 4:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-2.png";
                break;
            case 3:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-3.png";
                break;
            case 2:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-4.png";
                break;
            case 1:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-5.png";
                break;
            default:
                m_NowImagePath = "../Resources/Image/Player/Rectangle-6.png";
                break;
        }
    }else {
        throw std::invalid_argument("invalid max health");
    }
    if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
        imageDrawable->SetImage(m_NowImagePath);
    }
}


void Player::Squash() {
    glm::vec2 targetScale;
    if (m_Dashing && m_MovingDirection != glm::vec2(0.0f ,0.0f)) {  //衝刺
        //壓縮
        targetScale = glm::vec2(1.5f*normalScale, 0.5f*normalScale);
    }else if (m_MovingDirection == glm::vec2(0.0f ,0.0f)) {  //停止
        //回彈
        targetScale = glm::vec2(normalScale, normalScale);
    }else {  //移動
        //壓縮
        targetScale = glm::vec2(1.2f*normalScale, 0.8f*normalScale);
    }

    float dt = Util::Time::GetDeltaTimeMs() / 1000.0f;

    // 可自行微調的彈簧參數：
    float tension = 500.0f; // 拉力：數值越大，回彈力道越猛烈
    float damping = 15.0f;  // 阻尼：數值越大，晃動停止得越快

    // 虎克定律物理公式：力 = (目標與當前的距離 * 拉力)
    glm::vec2 force = (targetScale - m_Transform.scale) * tension;

    // 加速度影響速度 (減去阻尼摩擦力避免永遠停不下來)
    m_ScaleVelocity += (force - m_ScaleVelocity * damping) * dt;

    // 速度影響最終形狀 [4]
    m_Transform.scale += m_ScaleVelocity * dt;
}

void Player::Turn() {
    const float PI = std::acos(-1);
    float targetAngle;
    if (m_MovingDirection == glm::vec2(0.0f ,0.0f)) {  //停止
        //轉回來
        targetAngle = 0;
    }else {  //移動
        //面相行徑方向
        targetAngle = std::atan2(-m_MovingDirection.y, -m_MovingDirection.x);
    }
    float currentAngle = m_Transform.rotation;
    float diff = targetAngle - currentAngle;

    while (diff > PI)  diff -= 2*PI;
    while (diff < -PI) diff += 2*PI;

    float omega = 0.02;
    float step = omega * Util::Time::GetDeltaTimeMs();
    if (std::abs(diff) <= step) {
        m_Transform.rotation = targetAngle;
    }else {
        if (diff > 0) {
            m_Transform.rotation += step;
        } else {
            m_Transform.rotation -= step;
        }
    }
}
