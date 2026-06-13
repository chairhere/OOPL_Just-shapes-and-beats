//
// Created by cheese on 2026/4/17.
//

#include "EventObject/DiscreteSlider.hpp"

#include "Manager/MusicPlayerManager.hpp"

DiscreteSlider::DiscreteSlider(float startX, float startY, float space) {
    m_Transform.translation = glm::vec2(startX, startY);
    float currentX = startX;
    float currentY = startY;
    for (int i = 0 ; i < m_TotalSteps ; i++) {
        auto block = std::make_shared<Button>(b_gray);
        m_Blocks.push_back(block);
        block->SetOnClick([this, i]() {
            SetValue(i);
            if (not m_IsDragging) {
                m_IsDragging = true;
            }
        });
        block->SetOnHovering([this, i]() {
            if (m_IsDragging) {
                SetValue(i);
            }
        });
        block->FocusEnable(false);
        block->HoverEnable(false);
        block->m_Transform.translation = glm::vec2(currentX, currentY);
        currentX += block->GetScaledSize().x + space;
        m_Children.push_back(block);
    }

    m_MemStep = m_CurrentStep;
    UpdateVisuals();
}

glm::vec2 DiscreteSlider::GetSize() {
    return m_Blocks[0]->GetScaledSize();
}


void DiscreteSlider::SetOnValueChange(std::function<void(int)> event) {
    m_OnValueChanged = event;
}

void DiscreteSlider::SetValue(int step) {
    if (step < 0 || step > 20) return;
    m_CurrentStep = step;
}

void DiscreteSlider::UpdateVisuals() {
    for (int i = 0 ; i < m_TotalSteps ; i++) {
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
    const bool hovering = isHovering();
    const bool focused = isFocus();

    // ==========================================
    // 1. 處理互動邏輯 (獨立於視覺狀態之外！)
    // ==========================================
    if (hovering) {
        if (m_OnHover) m_OnHover();
        // 只要滑鼠在上面，不管 HoverEnable 是不是 false，都可以點擊！
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            if (m_OnClick) m_OnClick();
        }
    } else if (focused) {
        if (m_OnFocus) m_OnFocus();
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            if (m_OnClick) m_OnClick();
        }
    }
    for (auto block : m_Blocks) {
        block->Update();
    }
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