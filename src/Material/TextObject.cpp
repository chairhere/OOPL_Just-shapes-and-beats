//
// Created by cheese on 2026/4/5.
//

#include "Material/TextObject.hpp"

void TextObject::SetText(const std::string &text) {
    if (auto textDrawable = std::dynamic_pointer_cast<Util::Text>(m_Drawable)) {
        textDrawable->SetText(text);
    }
}
