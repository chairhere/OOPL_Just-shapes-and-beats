//
// Created by cheese on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_BUTTON_HPP
#define JUST_SHAPES_AND_BEATS_BUTTON_HPP
#include <functional>

#include "Util/GameObject.hpp"
#include "Util/Input.hpp"

class Button : public Util::GameObject {
public:
    Button() {}

    void SetOnClick(const std::function<void()> &onClickEvent);

    void SetOnHovering(const std::function<void()> &onHoverEvent);

    void SetOnFocus(const std::function<void()> &onFocusEvent);

    bool isHovering();

    void Focus() {m_Focus = true;}

    void Unfocus() {m_Focus = false;}

    bool isFocus() {return m_Focus;}

    void Update();

private:
    bool m_Focus = false;
    std::function<void()> m_OnClick = nullptr;
    std::function<void()> m_OnHover = nullptr;
    std::function<void()> m_OnFocus = nullptr;
};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP