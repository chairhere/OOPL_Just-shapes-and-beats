//
// Created by cheese on 2026/3/20.
//

#include "../include/Button.hpp"

// --- 建構子實作 ---
Button::Button(const std::string& fontPath, int fontSize, const std::string& text, const Util::Color& color, bool isSaver) {
    // 建立一個文字繪圖物件，指派給從 GameObject 繼承來的 m_Drawable
    m_Drawable = std::make_shared<Util::Text>(fontPath, fontSize, text, color);
    if (not isSaver) {
        normalState = std::make_shared<Button>(fontPath, fontSize, text, color, true);
    }
}

Button::Button(const std::string& imagePath, bool isSaver) {
    // 建立一個圖片繪圖物件，指派給從 GameObject 繼承來的 m_Drawable
    m_Drawable = std::make_shared<Util::Image>(imagePath);
    if (not isSaver) {
        normalState = std::make_shared<Button>(imagePath, true);
    }
}

// 動態修改文字
void Button::SetText(const std::string& text) {
    // 必須先確認目前的 m_Drawable 確實是 Util::Text 才能修改
    if (auto textDrawable = std::dynamic_pointer_cast<Util::Text>(m_Drawable)) {
        textDrawable->SetText(text);
    }
}

void Button::SetImage(const std::string& imagePath) {
    if (auto imageDrawable = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
        imageDrawable->SetImage(imagePath);
    }
}

// -----------------

void Button::SetOnClick(const std::function<void()> &onClickEvent) {
    m_OnClick = onClickEvent;
}

void Button::SetOnHovering(const std::function<void()> &onHoverEvent) {
    m_OnHover = onHoverEvent;
}

void Button::SetOnFocus(const std::function<void()> &onFocusEvent) {
    m_OnFocus = onFocusEvent;
}

bool Button::isHovering() {
    // [防護機制] 如果沒有設定 Drawable，就沒有碰撞體積，直接回傳 false
    if (m_Drawable == nullptr) {
        return false;
    }

    // 1. 取得滑鼠當前位置
    glm::vec2 mousePos = Util::Input::GetCursorPosition();

    // 2. 計算按鈕的實際寬高 (考量 Transform 的縮放)
    glm::vec2 size = m_Drawable->GetSize() * m_Transform.scale;
    glm::vec2 pos = m_Transform.translation;

    // 3. 簡單的碰撞偵測 AABB
    bool hovering = (mousePos.x >= pos.x - size.x / 2.0f &&
                     mousePos.x <= pos.x + size.x / 2.0f &&
                     mousePos.y >= pos.y - size.y / 2.0f &&
                     mousePos.y <= pos.y + size.y / 2.0f);

    return hovering;
}

void Button::Update() {
    if (isHovering()) {
        m_StateChanged = true;
        // [防護機制] 確保有設定 function 才執行，避免空指標崩潰
        if (m_OnHover) {
            m_OnHover();
        }

        // 使用 IsKeyDown 確保只有按下的那一幀觸發
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            if (m_OnClick) {
                m_OnClick();
            }
        }
    }else if (isFocus()) {
        m_StateChanged = true;
        if (m_OnFocus) {
            m_OnFocus();
        }
        if (Util::Input::IsKeyDown(Util::Keycode::RETURN)) {
            if (m_OnClick) {
                m_OnClick();
            }
        }
    }else if (m_StateChanged){
        m_StateChanged = false;
        m_Drawable;
    }
}