//
// Created by cheese on 2026/5/8.
//

#include "EventObject/SettingItem.hpp"

#include "Util/Image.hpp"

SettingItem::SettingItem(const std::string &text, std::shared_ptr<EventObject> object, Util::Color color) {
    // m_Drawable = std::make_shared<Util::Image>()
    //
    // m_Label = std::make_shared<TextObject>(25, text, color);
    // AddChild(m_Label);
    //
    // m_FunctionalObject = object;
}

void SettingItem::SetText(const std::string &text) {

}


void SettingItem::Update() {

}
