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
    void SetFocusImage(const std::string& hoverImagePath);

    void Update() override;

private:
    std::string m_NormalImagePath = "";  // 備份原始圖片路徑
    std::string m_FocusImagePath = "";   // 觸發時的圖片路徑
};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP