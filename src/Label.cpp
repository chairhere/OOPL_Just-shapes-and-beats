//
// Created by cheese on 2026/4/5.
//

#include "Label.hpp"

#include "Util/Input.hpp"

Label::Label(int fontSize, const std::string &text, const std::string &imagePath) {
    m_Text = std::make_shared<TextObject>(fontSize, text, Util::Color(255, 255, 255));
    m_Text->SetZIndex(1);
    m_Children.push_back(m_Text);

    m_BackgroundImage = std::make_shared<ImageObject>(imagePath);
    m_BackgroundImage->SetZIndex(0);
    m_Children.push_back(m_BackgroundImage);
}

void Label::SetText(const std::string &text) {
    m_Text->SetText(text);
}

void Label::SetImage(const std::string &imagePath) {
    m_BackgroundImage->SetImage(imagePath);
}

void Label::SetFocusText(const std::string &text) {
    m_FocusText = text;
}

void Label::SetFocusImage(const std::string &imagePath) {
    m_FocusImagePath = imagePath;
}

void Label::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();
    const bool isCurrentlyActive = isActive();

    if (isCurrentlyActive) {
        // 【剛碰到的第一幀】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;
            if (!m_FocusText.empty()) SetText(m_FocusText);
            if (!m_FocusImagePath.empty()) SetImage(m_FocusImagePath);
            m_WasActive = true;
        }

        // 觸發自定義的特效或點擊事件
        if (hovering) {
            if (m_OnHover) m_OnHover();
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                if (m_OnClick) m_OnClick();
            }
        } else if (focused) {
            if (m_OnFocus) m_OnFocus();
            if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
                if (m_OnClick) m_OnClick();
            }
        }
    } else {
        // 【離開的第一幀】還原所有狀態
        if (m_WasActive) {
            m_Transform = m_NormalTransform;
            if (!m_NormalText.empty() && !m_FocusText.empty()) SetText(m_NormalText);
            if (!m_NormalImagePath.empty() && !m_FocusImagePath.empty()) SetImage(m_NormalImagePath);
            if (!m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}

