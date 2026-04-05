//
// Created by cheese on 2026/4/5.
//

#ifndef JUST_SHAPES_AND_BEATS_TEXTOBJECT_HPP
#define JUST_SHAPES_AND_BEATS_TEXTOBJECT_HPP
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class TextObject : public Util::GameObject {
public:
    TextObject(int fontSize, const std::string& text, const Util::Color& color) {
        m_Drawable = std::make_shared<Util::Text>("../Resources/Font/TSTC.ttf", fontSize, text, color);
    }

    void SetText(const std::string& text);
};

#endif //JUST_SHAPES_AND_BEATS_TEXTOBJECT_HPP