//
// Created by jerry on 2026/3/20.
//

#include "LevelSpawner.hpp"

void LevelSpawner::Start() {
    std::ifstream file(m_BeatMap);
    if (!file.is_open()) {
        LOG_DEBUG("找不到譜面檔案：{}", m_BeatMap);
        return;
    }
    LOG_DEBUG("LoginLevelSpawner_Start");
    json m_LevelData;
    file >> m_LevelData;

    for (const auto& item : m_LevelData) {
        LOG_DEBUG("startbuild");
        SpawnEvent m_LoadEvent;
        //資料進來後根據障礙種類去做分類
        m_LoadEvent.StartBeat = static_cast<float>(item["StartBeat"]);
        m_LoadEvent.StartRot = item["StartRotation"];
        m_LoadEvent.StartPos = {item["StartPos"]["X"], item["StartPos"]["Y"]};
        if (item["ObstacleType"] == "RotatingRectangle") {
            m_LoadEvent.ShapeType = BulletType::RotatingRectangle;
            m_LoadEvent.SpecialData.Velocity.x = item["Velocity"];
            m_LoadEvent.SpecialData.Velocity.y = item["Velocity"];
            m_LoadEvent.SpecialData.AngularVelocity = item["AngularVelocity"];
            //m_LoadEvent.EndPos = {item["EndPos"]["X"], item["EndPos"]["Y"]};
            //m_LoadEvent.EndRot = item["EndRotation"];
            m_LoadEvent.EndBeat = item["EndBeat"];
        }
        else if (item["ObstacleType"] == "Laser") {
            m_LoadEvent.ShapeType = BulletType::Laser;
            m_LoadEvent.SpecialData.SpawnBeat = item["SpawnBeat"];
            m_LoadEvent.EndPos = {item["EndPos"]["X"], item["EndPos"]["Y"]};
            m_LoadEvent.EndRot = item["EndRotation"];
            m_LoadEvent.EndBeat = item["EndBeat"];
        }
        //else if (item["ObstacleType"] == "Spline") {}
        m_PendingEvents.push(m_LoadEvent);
    }
    LOG_DEBUG("finishedbuild");

}

//能實作在AppUpdate裡，利用levels來去開啟予與關閉這部分的update
void LevelSpawner::Update(float currentBeat, glm::vec2 PlayerPos) {
    LOG_DEBUG("LevelSpawner_Update", currentBeat);
    // 1. 檢查是否有新障礙物需要生成

    while (!m_PendingEvents.empty() && currentBeat >= m_PendingEvents.front().StartBeat) {
        SpawnEvent m_SpawnEvent;
        m_SpawnEvent = m_PendingEvents.front();

        if (m_SpawnEvent.ShapeType == BulletType::RotatingRectangle) {
            m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
            Obstacle newObstacle(m_SpawnEvent, m_SpawnVertices);
            newObstacle.customBehavior = [](Obstacle& self, float beat) {
                // 覆寫 X 軸位移，以原設定的 X 軸為基準，加上 Sin 波形
                self.m_Transform.rotation = beat * self.m_Event.SpecialData.AngularVelocity;
                self.m_Transform.translation = {self.m_Event.StartPos.x + self.m_Event.SpecialData.Velocity.x * (beat - self.m_Event.StartBeat), self.m_Event.StartPos.y + self.m_Event.SpecialData.Velocity.y * (beat - self.m_Event.StartBeat)};
            };
            m_ActiveObstacles.push_back(newObstacle);
        }
        else if (m_SpawnEvent.ShapeType == BulletType::Laser) {
            m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
            Obstacle newObstacle(m_SpawnEvent, m_SpawnVertices);
            newObstacle.customBehavior = [](Obstacle& self, float beat) {
                // 覆寫 X 軸位移，以原設定的 X 軸為基準，加上 Sin 波形
                if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < (self.m_Event.SpecialData.SpawnBeat + 1)) {
                    self.m_Transform.scale = {2000 * (beat - self.m_Event.SpecialData.SpawnBeat), 200};
                }else if (beat > (self.m_Event.SpecialData.SpawnBeat + 1)) {
                    self.m_Transform.scale = {2000, 200};
                }
            };
            m_ActiveObstacles.push_back(newObstacle);
        }

        //m_ActiveObstacles.push_back(newObstacle);

        // 將 newObstacle 加入您的 Util::Renderer 準備渲染 [6]
        // renderer.AddChild(newObstacle);

        m_PendingEvents.pop(); // 移除已生成的事件
    }
    m_Batcher->BeginBatch();
    // 2. 更新所有存活的障礙物狀態，並清理過期的障礙物
    for (auto it = m_ActiveObstacles.begin(); it != m_ActiveObstacles.end(); ) {

        if (it->m_Event.EndBeat < currentBeat) {
            it = m_ActiveObstacles.erase(it);
            continue;
        }

        it->UpdateStateByBeat(currentBeat, PlayerPos);

        // 生命週期管理：如果音樂已經超過了它的存活時間，立刻將其刪除
        /*
        if (it->IsDead()) {
            it = m_ActiveObstacles.erase(it);
            continue;
        }
        */

        // 執行客製化行為 (例如：隨機抖動、追蹤)
        if (it->customBehavior != nullptr) {
            it->customBehavior(*it, currentBeat);
        }

        // 物理與渲染更新 [3]

        it->UpdateWorldVertices();



        // 彙整到批次渲染器中 (假設您的 batcher 吃頂點與顏色) [6]
        m_Batcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs(), it->GetLocalVertices());

        ++it;
    }

    m_Batcher->EndBatch(); // 結束收集，交由 Renderer 自動呼叫 Draw()
}