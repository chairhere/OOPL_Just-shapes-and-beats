//
// Created by jerry on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP
#define JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP

#include <utility>
#include <vector>
#include <memory>
#include "Obstacle.hpp"
#include "SpawnEvent.hpp"

class LevelSpawner {
private:
    std::vector<SpawnEvent> m_PendingEvents; // 尚未生成的事件清單 (需依 startBeat 排序)
    std::vector<std::shared_ptr<Obstacle>> m_ActiveObstacles; // 畫面上存活的障礙物

public:
    explicit LevelSpawner(std::vector<SpawnEvent> pendingEvents) {
        m_PendingEvents = std::move(pendingEvents);
    };
    ~LevelSpawner();



};

#endif //JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP