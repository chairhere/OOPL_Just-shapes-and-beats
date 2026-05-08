//
// Created by cheese on 2026/5/8.
//

#ifndef JUST_SHAPES_AND_BEATS_SETTINGITEM_HPP
#define JUST_SHAPES_AND_BEATS_SETTINGITEM_HPP
#include "EventObject.hpp"
#include "Material/TextObject.hpp"

class SettingItem : public EventObject {
public:
    SettingItem(const std::string& text, std::shared_ptr<EventObject> object, Util::Color color);

    void SetText(const std::string &text);
    void SetFocusImage(const std::string &imagePath);

    void Update() override;

private:
    std::shared_ptr<TextObject> m_Label;
    std::shared_ptr<EventObject> m_FunctionalObject;

    std::string m_NormalImagePath = "Resources/Image/OptionBackground/None.png";  // 備份原始圖片路徑
    std::string m_FocusImagePath = "Resources/Image/OptionBackground/Selected.png";   // 觸發時的圖片路徑
};

#endif //JUST_SHAPES_AND_BEATS_SETTINGITEM_HPP