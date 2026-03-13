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
        Moving_Direction += glm::vec2(0.0f, 1.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S) or Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        Moving_Direction += glm::vec2(0.0f, -1.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A) or Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        Moving_Direction += glm::vec2(-1.0f, 0.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D) or Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        Moving_Direction += glm::vec2(1.0f, 0.0f);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        Dash();
    }
    if (Dashing) {
        Moving_Direction *= 5;
        Dash_Time_Left -= Util::Time::GetDeltaTimeMs();
        if (Dash_Time_Left <= 0) {
            Dashing = false;
        }
    }
    MovePosition(Moving_Direction);
    Moving_Direction = glm::vec2(0.0f, 0.0f);
}

void Player::Dash() {
    Dashing = true;
    Dash_Time_Left = 500.0f;
}