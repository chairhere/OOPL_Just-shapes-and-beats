//
// Created by jerry on 2026/3/20.
//

#include "LevelSpawner.hpp"

//能實作在AppUpdate裡，利用levels來去開啟予與關閉這部分的update
void LevelSpawner::Update(float currentBeat) {
    // 1. 檢查是否有新障礙物需要生成
    while (!m_PendingEvents.empty() && currentBeat >= m_PendingEvents.front().startBeat) {
        auto newObstacle = std::make_shared<Obstacle>(m_PendingEvents.front());
        m_ActiveObstacles.push_back(newObstacle);

        // 將 newObstacle 加入您的 Util::Renderer 準備渲染 [6]
        // renderer.AddChild(newObstacle);

        m_PendingEvents.erase(m_PendingEvents.begin()); // 移除已生成的事件
    }

    // 2. 更新所有存活的障礙物狀態，並清理過期的障礙物
    for (auto it = m_ActiveObstacles.begin(); it != m_ActiveObstacles.end(); ) {
        (*it)->UpdateStateByBeat(currentBeat);

        if ((*it)->m_IsDead) {
            // 從 Renderer 中移除並清理記憶體 [6]
            // renderer.RemoveChild(*it);
            it = m_ActiveObstacles.erase(it);
        } else {
            ++it;
        }
    }
}