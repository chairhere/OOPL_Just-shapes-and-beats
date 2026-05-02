//
// Created by cheese on 2026/4/17.
//

#include "EventObject/DiscreteSlider.hpp"

DiscreteSlider::DiscreteSlider(float startX, float startY, float space) {
    m_Transform.translation = glm::vec2(startX, startY);
    float currentX = startX;
    float currentY = startY;
    for (int i = 0 ; i < m_TotalSteps ; i++) {
        auto block = std::make_shared<Button>(b_gray);
        m_Blocks.push_back(block);
        block->SetOnClick([this, i]() {
            SetValue(i);
            m_IsDragging = true;
        });
        block->SetOnHovering([this, i]() {
            if (m_IsDragging) {
                SetValue(i);
            }
        });
        m_Children.push_back(block);
    }

    m_MemStep = m_CurrentStep;
}

void DiscreteSlider::SetOnValueChange(std::function<void(int)> event) {
    m_OnValueChanged = event;
}

void DiscreteSlider::SetValue(int step) {
    if (step < 0 || step > 20) return;
    m_CurrentStep = step;
}

void DiscreteSlider::UpdateVisuals() {
    for (int i = 1 ; i <= m_TotalSteps ; i++) {
        if (i < m_CurrentStep) {
            m_Blocks.at(i)->SetImage(b_red);
        }else if (i == m_CurrentStep) {
            m_Blocks.at(i)->SetImage(b_white);
        }else {
            m_Blocks.at(i)->SetImage(b_gray);
        }
    }
}


void DiscreteSlider::Update() {
    if (m_MemStep != m_CurrentStep) {
        m_MemStep = m_CurrentStep;
        UpdateVisuals();
        if (m_OnValueChanged) {
            m_OnValueChanged(m_CurrentStep);
        }
    }
    if (m_IsDragging && not Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        m_IsDragging = false;
    }
}