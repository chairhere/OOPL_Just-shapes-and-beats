//
// Created by cheese on 2026/4/17.
//

#ifndef JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#define JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP
#include "EventObject.hpp"
#include "ImageObject.hpp"

class DiscreteSlider : public EventObject {
public:
    //20格 一格0.05
    DiscreteSlider(int total_steps, float startX, float startY, float space = 5.0f);

    void SetOnValueChange(std::function<void(int)> event);
    void SetValue(int step);

    void Update() override;    void UpdateVisuals(); // 統一處理方格亮起/熄滅的視覺更新
private:
    int m_TotalSteps;
    int m_CurrentStep = 0;
    bool m_IsDragging = false;

    // 儲存所有方格的陣列
    std::vector<std::shared_ptr<ImageObject>> m_Blocks;

    std::function<void(int)> m_OnValueChanged;
};

#endif //JUST_SHAPES_AND_BEATS_DISCRETESLIDER_HPP