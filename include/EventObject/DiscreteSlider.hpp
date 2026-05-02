//
// Created by cheese on 2026/4/17.
//

#ifndef JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#define JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#include "EventObject/Button.hpp"
#include "EventObject/EventObject.hpp"

class DiscreteSlider : public EventObject {
public:
    //20格 一格0.05
    DiscreteSlider(float startX, float startY, float space = 5.0f);

    void SetOnValueChange(std::function<void(int)> event);
    void SetValue(int step);

    void UpdateVisuals(); // 統一處理方格亮起/熄滅的視覺更新
    void Update() override;
private:
    int m_TotalSteps = 20;
    int m_CurrentStep = 0;
    int m_MemStep;
    bool m_IsDragging = false;

    // 儲存所有方格的陣列
    std::vector<std::shared_ptr<Button>> m_Blocks;

    std::function<void(int)> m_OnValueChanged;

    std::string b_red = "../Resources/Image/Slider/selected.png";
    std::string b_white = "../Resources/Image/Slider/current.png";
    std::string b_gray = "../Resources/Image/Slider/outlier.png";


};

#endif //JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP