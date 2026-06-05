//
// Created by cheese on 2026/4/5.
//

#ifndef JUST_SHAPES_AND_BEATS_MERGEBUTTON_HPP
#define JUST_SHAPES_AND_BEATS_MERGEBUTTON_HPP
#include "EventObject/EventObject.hpp"
#include "Material/ImageObject.hpp"
#include "Material/TextObject.hpp"
#include "Util/GameObject.hpp"
#include "Util/Text.hpp"

class MergeButton : public EventObject {
    // A button can custom background and text at the same time.
public:
    MergeButton(int fontSize, const std::string& text, const std::string& imagePath, Util::Color color = Util::Color(255, 255, 255));

    void SetText(const std::string &text);
    void SetImage(const std::string &imagePath);

    bool isHovering() const override;

    void SetFocusText(const std::string &text);
    void SetFocusImage(const std::string &imagePath);

    glm::vec2 GetSize();

    void Update() override;

private:
    std::shared_ptr<TextObject> m_Text;
    std::shared_ptr<ImageObject> m_BackgroundImage;

    std::string m_NormalText = "";  //備份原始文字
    std::string m_FocusText = "";  //觸發時文字
    std::string m_NormalImagePath = "";  // 備份原始圖片路徑
    std::string m_FocusImagePath = "";   // 觸發時的圖片路徑

};

#endif //JUST_SHAPES_AND_BEATS_MERGEBUTTON_HPP