#include "../include/Button.hpp"

// --- 建構子 ---
Button::Button(const std::string& fontPath, int fontSize, const std::string& text, const Util::Color& color) {
    m_Drawable = std::make_shared<Util::Text>(fontPath, fontSize, text, color);
}

Button::Button(const std::string& imagePath) {
    m_NormalImagePath = imagePath; // 記住預設的圖片路徑
    m_Drawable = std::make_shared<Util::Image>(imagePath);
}

// --- 屬性設定 ---
void Button::SetText(const std::string& text) {
    if (auto textDrawable = std::dynamic_pointer_cast<Util::Text>(m_Drawable)) {
        textDrawable->SetText(text);
    }
}

void Button::SetImage(const std::string& imagePath) {
    if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
        imageDrawable->SetImage(imagePath);
    }
}

void Button::SetFocusImage(const std::string& hoverImagePath) {
    m_FocusImagePath = hoverImagePath;
}

void Button::Update() {
    const bool hovering = isHovering();
    const bool focused = isFocus();
    const bool isCurrentlyActive = isActive();

    if (isCurrentlyActive) {
        // 【剛碰到的第一幀】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;
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
            if (!m_NormalImagePath.empty() && !m_FocusImagePath.empty()) {
                SetImage(m_NormalImagePath);
            }
            if (!m_OffEvent) {
              m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}