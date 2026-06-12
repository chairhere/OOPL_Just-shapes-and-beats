//
// Created by cheese on 2026/5/2.
//

#ifndef JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP
#include "EventObject/DiscreteSlider.hpp"
#include "EventObject/MergeButton.hpp"
#include "Material/ImageObject.hpp"
#include "Material/TextObject.hpp"
#include "Screen/Screen.hpp"

class SettingScreen : public Screen {
public:
    ~SettingScreen() = default;

    enum class Page {
        Volume
    };

    SettingScreen();

    void Call(ScreenState WhoCalls);
    void HangUp();

    ScreenState Update() override;

private:
    Page m_CurrentPage;
    std::shared_ptr<ImageObject> m_Background;
    std::vector<std::shared_ptr<ImageObject>> m_LeftLines;

    glm::vec2 LeftFirst = glm::vec2(-519.0f, 293.0f);
    glm::vec2 RightFirst = glm::vec2(-163.0f, 277.0f);
    glm::vec2 LeftWhere = glm::vec2(-519.0f, 293.0f);
    glm::vec2 RightWhere = glm::vec2(-163.0f, 277.0f);
    static constexpr float gap = 10.0f;

    std::shared_ptr<MergeButton> m_VolumePage;
    std::shared_ptr<MergeButton> m_Return;
    std::shared_ptr<MergeButton> m_Restart;
    std::shared_ptr<MergeButton> m_LeaveToList;

    std::shared_ptr<TextObject> m_VolumeLabel;
    std::shared_ptr<TextObject> m_BGMLabel, m_SFXLabel;
    std::shared_ptr<DiscreteSlider> m_BGMSlider, m_SFXSlider;

    std::shared_ptr<Button> m_QuitGamePlay;

    const std::string none = "../Resources/Image/OptionBackground/None.png";
    const std::string selected = "../Resources/Image/OptionBackground/Selected.png";
};

#endif //JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP