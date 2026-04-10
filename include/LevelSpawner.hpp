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
#include "TimeLine.hpp"
#include "BatchedColorShape.hpp"
#include "./lib/json.hpp"
#include <fstream>           // 用來讀取檔案
#include <iostream>
#include <queue>
#include "Util/Logger.hpp"   // 沿用你的 Log 系統
#include "config.hpp"
#include "Util/GameObject.hpp"
#include "BulletType.hpp"

using json = nlohmann::json;

class LevelSpawner : public Util::GameObject{
private:

    std::queue<SpawnEvent> m_PendingEvents; // 尚未生成的事件清單 (需依 startBeat 排序)
    std::vector<Obstacle> m_ActiveObstacles; // 畫面上存活的障礙物
    std::shared_ptr<BatchedColorShape> m_Batcher;

    std::shared_ptr<TimeLine> m_TimeLine;
    std::string m_BeatMap;
    std::string m_SongPath;

    bool m_IsFinished = false;

public:
    explicit LevelSpawner(const std::string& filepath, const std::string& SongPath, const float BPM){
        m_Batcher = std::make_shared<BatchedColorShape>(Util::Color{255, 33, 111, 255});
        m_BeatMap = filepath;
        m_SongPath = SongPath;
        m_TimeLine = std::make_shared<TimeLine>(m_SongPath, BPM);
        m_Drawable = m_Batcher;
    };
    ~LevelSpawner() override = default;

    void Update(float currentBeat);

    void Start();


    bool IsFinished() const {return m_IsFinished;}
};

#endif //JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP