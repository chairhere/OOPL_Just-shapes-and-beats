#ifndef JUST_SHAPES_AND_BEATS_BUTTON_HPP
#define JUST_SHAPES_AND_BEATS_BUTTON_HPP

#include <functional>
#include <string>

#include "EventObject.hpp"
#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/Color.hpp"

class Button : public EventObject {
public:
    Button(const std::string& fontPath, int fontSize, const std::string& text, const Util::Color& color = Util::Color(255, 255, 255));
    explicit Button(const std::string& imagePath);

    void SetText(const std::string& text);
    void SetImage(const std::string& imagePath);

    // [新增] 設定觸發(Hover/Focus)時的替換圖片
    void SetHoverImage(const std::string& hoverImagePath);

    void Update() override;

};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP