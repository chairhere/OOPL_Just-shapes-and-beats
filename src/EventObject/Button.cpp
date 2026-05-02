#include "EventObject/Button.hpp"

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

    // ==========================================
    // 2. 處理視覺狀態 (這裡才套用 HoverEnable / FocusEnable 的限制)
    // ==========================================
    // 重新定義：這個物件「視覺上」是否該呈現亮起狀態？
    const bool shouldShowVisualActive = (m_HoverEnable && hovering) || (m_FocusEnable && focused);

    if (shouldShowVisualActive) {
        // 【該亮起時】進行狀態快照與自動換圖
        if (!m_WasActive) {
            m_NormalTransform = m_Transform;
            if (!m_FocusImagePath.empty()) {
                SetImage(m_FocusImagePath);
            }
            m_WasActive = true;
        }
    } else {
        // 【該熄滅時】還原所有狀態
        if (m_WasActive) {
            m_Transform = m_NormalTransform;
            if (!m_NormalImagePath.empty()) {
                SetImage(m_NormalImagePath);
            }
            if (m_OffEvent) {
                m_OffEvent();
            }
            m_WasActive = false;
        }
    }
}