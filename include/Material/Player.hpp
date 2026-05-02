//
// Created by cheese on 2026/3/13.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYER_HPP
#define JUST_SHAPES_AND_BEATS_PLAYER_HPP
#include "Material/ImageObject.hpp"
#include "glm/vec2.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"

class Player : public Util::GameObject {
public:
    Player();

    [[nodiscard]] glm::vec2 GetPosition() {return m_Transform.translation;}

    [[nodiscard]] float GetRotation() {return m_Transform.rotation;}

    [[nodiscard]] glm::vec2 GetScale() {return m_Transform.scale;}

    void SetPosition(glm::vec2 new_position);

    void MovePosition(glm::vec2 movement);

    void SetRotation(float arc);

    bool Moving();  //回傳是否死亡

    void Dash();
    void Hit();
    void Shake( glm::vec2 movement);

    void Die();
    void Revive();

protected:
    int m_MaxHealth = 3;
    int m_Health = m_MaxHealth;

    const float m_Speed = 5.0f;
    glm::vec2 m_MovingDirection = glm::vec2(0.0f, 0.0f);
    bool m_Dashing = false;
    float m_DashTimeLeft = 0;
    bool m_DashCoolDown = false;
    std::string m_DashGlowImagePath = "../Resources/Image/Dash/Player1_Dash.png";
    std::shared_ptr<ImageObject> m_Background;

    bool m_Invincible = false;
    float m_InvincibleTimeLeft = 0;
    bool m_KnockBack = false;
    glm::vec2 m_KnockBackDirection = glm::vec2(0.0f, 0.0f);
    bool m_Stun = false;
    bool m_Blink = false;
    std::string m_NowImagePath = "../Resources/Image/Player/Rectangle.png";
    std::string m_BlinkImagePath = "../Resources/Image/Player/Rectangle-blink.png";
    void ChangeImage();


    bool m_NoDamage = true;
    float m_NoDamageTimeLeft = 1000.0f;
    glm::vec2 m_LastOffset = glm::vec2(0.0f, 0.0f);

    void Squash();
    glm::vec2 m_ScaleVelocity = glm::vec2(0.0f, 0.0f);  //彈性
    void Turn();
    float normalScale = 0.5f;

};

#endif //JUST_SHAPES_AND_BEATS_PLAYER_HPP