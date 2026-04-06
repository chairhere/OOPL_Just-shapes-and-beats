//
// Created by cheese on 2026/4/6.
//

#ifndef JUST_SHAPES_AND_BEATS_SONGLISTITEM_HPP
#define JUST_SHAPES_AND_BEATS_SONGLISTITEM_HPP
#include "EventObject.hpp"
#include "ImageObject.hpp"
#include "Levels.hpp"
#include "SongData.hpp"
#include "TextObject.hpp"

class SongListItem : public EventObject {
public:
    SongListItem(SongData data, float startX, float startY);

    std::string_view GetName();

    Levels GetLevel();

    void Update() override;

private:
    std::shared_ptr<ImageObject> m_Background, m_Checker;
    std::shared_ptr<TextObject> m_Title, m_Composer;

    Levels m_WhoAmI;
    bool m_Checked = false;
    int m_Order = -1;  //not checked

    std::string m_NormalBackground = "../Resources/Image/OptionBackground/None.png";
    std::string m_FocusBackground = "../Resources/Image/OptionBackground/Selected.png";
};

#endif //JUST_SHAPES_AND_BEATS_SONGLISTITEM_HPP