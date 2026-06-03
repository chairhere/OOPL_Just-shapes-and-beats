//
// Created by jerry on 2026/3/20.
//

#ifndef JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP
#define JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP

#include <utility>
#include <vector>
#include <memory>
#include <random>
#include "Material/Obstacle.hpp"
#include "Spawner/SpawnEvent.hpp"
#include "Spawner/BatchedColorShape.hpp"
#include "Spawner/BatchedCircleShape.hpp"
#include "Lib_json/json.hpp"
#include <fstream>           // 用來讀取檔案
#include <iostream>
#include <queue>
#include "Util/Logger.hpp"   // 沿用你的 Log 系統
#include "config.hpp"
#include "Util/GameObject.hpp"
#include "Util/TransformUtils.hpp"
#include "Tool/BulletType.hpp"

using json = nlohmann::json;

struct CompareEvent {
    // 必須實作 operator()
    bool operator()(const SpawnEvent& a, const SpawnEvent& b) const {
        // 重點：我們希望 StartBeat "最小" 的排在最前面
        // 所以當 a > b 時回傳 true，這會迫使優先權佇列把較小的元素往上推 (Min-Heap)
        return a.StartBeat > b.StartBeat;
    }
};

class LevelSpawner : public Util::GameObject{
private:

    std::queue<SpawnEvent> m_PendingEvents; // 尚未生成的事件清單 (需依 startBeat 排序)
    std::priority_queue<SpawnEvent, std::vector<SpawnEvent>, CompareEvent> m_WaitingEvets;
    std::vector<Obstacle> m_ActiveObstacles; // 畫面上存活的障礙物
    std::shared_ptr<BatchedColorShape> m_Batcher;
    std::shared_ptr<BatchedCircleShape> m_CircleBatcher;
    std::shared_ptr<BatchedCircleShape> m_SpikeBatcher;
    std::shared_ptr<BatchedCircleShape> m_DottedCircleBatcher;
    std::shared_ptr<BatchedCircleShape> m_DottedLineBatcher;

    std::string m_BeatMap;

    std::vector<float> m_SpawnVertices;

    glm::vec2 m_ShakeOffset = {0.0f, 0.0f};
    glm::vec2 m_JitterOffset = {0.0f, 0.0f};

    float m_StartShakeBeat = 0.0f;
    float m_CurrentOffset = 0.0f;
    const float s_ShakeDuration = 0.25f;
    const float s_Amplitude = 5.0f;

    float t1 = 0.0f;
    // 執行 2. 關卡排程系統 (UpdateLevel)
    float t2 = 0.0f;
    // 執行 3. 物件與變換系統 (包含渲染 DrawCall)
    float t3 = 0.0f;
    // 執行 4. 數學與碰撞系統 (AABB / 距離判斷)
    float t4 = 0.0f;

    int m_ObstaclesCount = 0;
    int m_PoolIndex = 0;

    int S_PoolSize;

    bool m_IsFinished = false;
    bool m_IsColliding = false;
    bool m_IsChecked = false;
    bool m_IsJitter = false;

    std::random_device rd;  //隨機種子
    std::mt19937 g = std::mt19937(rd());  //取亂數

    Util::Color Blue = Util::Color{151, 215, 233, 255};
    Util::Color Red = Util::Color{255, 33, 111, 255};

public:
    explicit LevelSpawner(const std::string& filepath){
        m_Batcher = std::make_shared<BatchedColorShape>(Util::Color{255, 33, 111, 255});
        m_CircleBatcher = std::make_shared<BatchedCircleShape>(Util::Color{255, 33, 111, 255});
        m_SpikeBatcher = std::make_shared<BatchedCircleShape>(Util::Color{255, 33, 111, 255});
        m_DottedCircleBatcher = std::make_shared<BatchedCircleShape>(Util::Color{255, 33, 111, 255});
        m_DottedLineBatcher = std::make_shared<BatchedCircleShape>(Blue);
        m_BeatMap = filepath;
        m_Drawable = m_Batcher;
    };
    ~LevelSpawner() override = default;

    void Update(float currentBeat, glm::vec2 PlayerPos);

    void Start(float StartBeat = 0.0f);

    Obstacle* GetActiveObstacle();

    bool IsFinished() const {return m_IsFinished;}

    bool IsColliding() const {return m_IsColliding;}

    bool IsChecked() const {return m_IsChecked;}

    void VisionShake(glm::vec2 value, float currentBeat);

    void CreateObstacle(SpawnEvent m_SpawnEvent, glm::vec2 PlayerPos);

    glm::vec2 GetCurrentShakeOffset() const {return m_Transform.translation;}

    int GetObstaclesCount(){return m_ObstaclesCount;};

    int GetWaitingObstacleIndex(){return m_PoolIndex;}


    void DrawAll();
};



#endif //JUST_SHAPES_AND_BEATS_LEVELSPAWNER_HPP