#ifndef JUST_SHAPES_AND_BEATS_BUTTON_HPP
#define JUST_SHAPES_AND_BEATS_BUTTON_HPP

#include <functional>
#include <string>

#include "Util/GameObject.hpp"
#include "Util/Input.hpp"
#include "Util/Text.hpp"
#include "Util/Image.hpp"
#include "Util/Color.hpp"

class Button : public Util::GameObject {
public:
    // 全域共用的鍵盤模式標記，用來防止 Hover 雙重觸發
    static bool s_IsKeyboardMode;

    Button(const std::string& fontPath, int fontSize, const std::string& text, const Util::Color& color = Util::Color(255, 255, 255));
    explicit Button(const std::string& imagePath);

    void SetText(const std::string& text);
    void SetImage(const std::string& imagePath);

    // [新增] 設定觸發(Hover/Focus)時的替換圖片
    void SetHoverImage(const std::string& hoverImagePath);

    void SetOnClick(const std::function<void()> &onClickEvent);
    void SetOnHovering(const std::function<void()> &onHoverEvent);
    void SetOnFocus(const std::function<void()> &onFocusEvent);

    bool isHovering();

    void Focus() { m_Focus = true; }
    void Unfocus() { m_Focus = false; }
    bool isFocus() { return m_Focus; }

    void Update();

private:
    bool m_Focus = false;
    std::function<void()> m_OnClick = nullptr;
    std::function<void()> m_OnHover = nullptr;
    std::function<void()> m_OnFocus = nullptr;

    // --- 自動還原系統 (Snapshot) 變數 ---
    bool m_WasActive = false;
    Util::Transform m_NormalTransform;   // 備份位移、縮放、旋轉
    std::string m_NormalImagePath = "";  // 備份原始圖片路徑
    std::string m_HoverImagePath = "";   // 觸發時的圖片路徑
};

#endif //JUST_SHAPES_AND_BEATS_BUTTON_HPP