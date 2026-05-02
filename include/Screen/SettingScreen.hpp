//
// Created by cheese on 2026/5/2.
//

#ifndef JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP
#define JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP
#include "EventObject/DiscreteSlider.hpp"
#include "Screen/Screen.hpp"

class SettingScreen : public Screen {
public:
    ScreenState Update() override;

private:
    std::shared_ptr<Button> m_VolumePage;
    std::shared_ptr<DiscreteSlider> m_BGMSlider, m_SFXSlider;

    std::shared_ptr<Button> m_QuitGamePlay;
};

#endif //JUST_SHAPES_AND_BEATS_SETTINGSCREEN_HPP