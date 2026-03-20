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

    bool isHovering();

    void FocusOn() {m_OnFocus = true;}

    void FocusOff() {m_OnFocus = false;}

    bool isFocus() {return m_OnFocus;}

private:
    bool m_OnFocus = false;
    std::function<void()> m_OnClick = nullptr;
};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP