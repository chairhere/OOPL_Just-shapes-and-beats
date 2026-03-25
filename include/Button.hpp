//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_BUTTON_HPP
#define JUST_SHAPES_AND_BEATS_BUTTON_HPP

#include <functional>
#include <string>
#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Text.hpp"   // 引入文字系統
#include "Util/Image.hpp"  // 引入圖片系統
#include "Util/Color.hpp"  // 引入顏色系統

class Button : public Util::GameObject {
public:
    // [基礎補完 1] 提供建構子：純文字按鈕
    // 利用 Util::Text 初始化 m_Drawable
    Button(const std::string& fontPath, int fontSize, const std::string& text, const Util::Color& color = Util::Color(255, 255, 255), bool isSaver = false);

    // [基礎補完 2] 提供建構子：圖片按鈕
    // 利用 Util::Image 初始化 m_Drawable
    explicit Button(const std::string& imagePath, bool isSaver = false);

    // [進階功能] 提供動態修改文字的功能
    void SetText(const std::string& text);
    void SetImage(const std::string& imagePath);

    void SetOnClick(const std::function<void()> &onClickEvent);
    void SetOnHovering(const std::function<void()> &onHoverEvent);
    void SetOnFocus(const std::function<void()> &onFocusEvent);

    std::function<void()> OnClickEvent() {return m_OnClick;}
    std::function<void()> OnHoverEvent() {return m_OnHover;}
    std::function<void()> OnFocusEvent() {return m_OnFocus;}

    bool isHovering();

    void Focus() { m_Focus = true; }
    void Unfocus() { m_Focus = false; }
    bool isFocus() { return m_Focus; }

    void Update();

private:
    bool m_Focus = false;
    bool m_StateChanged = false;
    std::function<void()> m_OnClick = nullptr;
    std::function<void()> m_OnHover = nullptr;
    std::function<void()> m_OnFocus = nullptr;
    std::shared_ptr<Button> normalState;
};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP