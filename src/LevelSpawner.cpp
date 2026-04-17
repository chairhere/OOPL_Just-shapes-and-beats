//
// Created by jerry on 2026/3/20.
//

#include "LevelSpawner.hpp"

void LevelSpawner::Start() {
    g = std::mt19937(rd());

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
            m_LoadEvent.SpecialData.Velocity = item["Velocity"];
            m_LoadEvent.SpecialData.AngularVelocity = item["AngularVelocity"];
            m_LoadEvent.EndBeat = item["EndBeat"];
            m_LoadEvent.Scale = glm::vec2{15.0f, 15.0f};
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
    m_ActiveObstacles.resize(2000);
    m_IsColliding =  m_ActiveObstacles[0].IsActive();
    LOG_DEBUG("finishedbuild");

}

Obstacle* LevelSpawner::GetActiveObstacle() {
    for (auto& obs : m_ActiveObstacles) {
        if (!obs.IsActive()) {
            return &obs;
        }
    }
    // 如果池子滿了，可以選擇擴充或是報錯 (通常建議把 m_PoolSize 設大一點)

    return nullptr;
}

void LevelSpawner::VisionShake(glm::vec2 value, float currentBeat) {
    m_StartShakeBeat = currentBeat;
    m_ShakeOffset = value;
}


//能實作在AppUpdate裡，利用levels來去開啟予與關閉這部分的update
void LevelSpawner::Update(float currentBeat, glm::vec2 PlayerPos) {
    m_IsColliding = false;
    // 1. 檢查是否有新障礙物需要生成

    if (m_StartShakeBeat + s_ShakeDuration * 2 >= currentBeat) {
        m_CurrentOffset = (s_ShakeDuration - std::abs( 4 * s_ShakeDuration * (currentBeat - m_StartShakeBeat) - s_ShakeDuration));
        m_Transform.translation = {m_ShakeOffset.x * m_CurrentOffset, m_ShakeOffset.y * m_CurrentOffset};
    }else {
        m_Transform.translation = {0.0f, 0.0f};
    }

    while (!m_PendingEvents.empty() && currentBeat >= m_PendingEvents.front().StartBeat) {
        SpawnEvent m_SpawnEvent;
        m_SpawnEvent = m_PendingEvents.front();

        if (m_SpawnEvent.ShapeType == BulletType::RotatingRectangle) {
            m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

            float dist = glm::length(glm::vec2{(PlayerPos.x - m_SpawnEvent.StartPos.x), (PlayerPos.y - m_SpawnEvent.StartPos.y)});
            float theta = -glm::atan((PlayerPos.x - m_SpawnEvent.StartPos.x) / (PlayerPos.y - m_SpawnEvent.StartPos.y)) +
                (glm::pi<float>() - glm::sign((PlayerPos.y - m_SpawnEvent.StartPos.y)) * (glm::pi<float>() / 2));
            std::uniform_real_distribution<float> dis(0.1745f, 0.7854f);
            std::uniform_real_distribution<float> val(0.9f, 1.1f);
            float thetaUp = theta + dis(g);
            float thetaDown = theta - dis(g);
            float velocityWeight = val(g);
            std::vector<float> thetas = {theta, thetaDown, thetaUp};
            for (int i = 0; i < 3; i++) {
                m_SpawnEvent.SpecialData.radian = glm::vec2{glm::cos(thetas[i]), glm::sin(thetas[i])};
                m_SpawnEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * velocityWeight;
                Obstacle* newObs = GetActiveObstacle();


                newObs->customBehavior = [](Obstacle& self, float beat, glm::vec2 PlayerPos) {
                    // 覆寫 X 軸位移，以原設定的 X 軸為基準，加上 Sin 波形

                    self.m_Transform.rotation = beat * self.m_Event.SpecialData.AngularVelocity;

                    self.m_Transform.translation = self.m_Event.StartPos +
                        glm::vec2{self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.radian.x * (beat - self.m_Event.StartBeat),
                            self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.radian.y * (beat - self.m_Event.StartBeat)};

                    self.UpdateWorldVertices();

                    self.m_IsColliding = self.CheckCollision(PlayerPos);
                };
                newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
            }

        }
        else if (m_SpawnEvent.ShapeType == BulletType::Laser) {
            m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
            Obstacle* newObs = GetActiveObstacle();
            newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
                float m_DetlaBeat = beat - self.GetLastBeat();
                float m_GapBeat = glm::abs(self.m_Event.SpecialData.SpawnBeat - beat);
                // 覆寫 X 軸位移，以原設定的 X 軸為基準，加上 Sin 波形
                if (beat <self.m_Event.SpecialData.SpawnBeat) {

                    self.m_Transform.scale = {4000, 20 * beat / self.m_Event.SpecialData.SpawnBeat};
                    float m_ColorValue = 0.25f + std::fmod(beat, 0.25f);
                    std::vector<float> warningUvs = {m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16};
                    self.SetUvs(warningUvs);
                }
                else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < (self.m_Event.SpecialData.SpawnBeat + 0.25)) {
                    self.TurnOnCollidable();
                    if (!self.IsShaked()) {
                        this->VisionShake({-100 * glm::cos(self.m_Event.StartRot), -100 * glm::sin(self.m_Event.StartRot)}, self.m_Event.SpecialData.SpawnBeat);
                        self.HasShaked();
                    }
                    float m_TransferColor = 0.25f + ((0.5f + m_GapBeat * 2));
                    std::vector<float> Uvs = {m_TransferColor, 0.25f, m_TransferColor, 0.25f, m_TransferColor, 0.25f, m_TransferColor, 0.25f};
                    self.SetUvs(Uvs);
                    self.m_Transform.scale = {4000 * (beat - self.m_Event.SpecialData.SpawnBeat) * 4, 20 * (beat - self.m_Event.SpecialData.SpawnBeat) * 4};
                }else if (beat < (self.m_Event.SpecialData.SpawnBeat + 0.5)) {
                    self.m_Transform.scale = {4000, 20};
                }else if ( beat >= (self.m_Event.SpecialData.SpawnBeat + 0.5) && beat < (self.m_Event.SpecialData.SpawnBeat + 1)){

                    std::vector<float> Uvs = {0.25f, m_GapBeat - 0.25f, 0.25f, m_GapBeat - 0.25f, 0.25f, m_GapBeat - 0.25f, 0.25f, m_GapBeat - 0.25f};
                    self.SetUvs(Uvs);
                    self.m_Transform.scale = {4000, 20 * (2 - 2 * m_GapBeat)};
                }
                self.UpdateWorldVertices();

                self.m_IsColliding = self.CheckCollision(PlayerPos);
            };
            newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
        }

        //m_ActiveObstacles.push_back(newObstacle);

        // 將 newObstacle 加入您的 Util::Renderer 準備渲染 [6]
        // renderer.AddChild(newObstacle);

        m_PendingEvents.pop(); // 移除已生成的事件
    }
    m_Batcher->BeginBatch();
    // 2. 更新所有存活的障礙物狀態，並清理過期的障礙物
    for (auto it = m_ActiveObstacles.begin(); it != m_ActiveObstacles.end(); ) {

        if (it->m_Event.EndBeat < currentBeat || it->IsDead() || !it->IsActive()) {
            ++it;
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
            it->customBehavior(*it, currentBeat, PlayerPos);
        }

        // 物理與渲染更新 [3]
/*
        it->UpdateWorldVertices();
*/
        if (it->m_IsColliding) {
            m_IsColliding = true;
        }

        if (m_Transform.translation.x != 0.0f || m_Transform.translation.y != 0.0f) {
            std::vector<float> ramVertices = it->GetWorldVertices();
            for (int i = 0; i < it->GetWorldVertices().size() / 2; i++) {
                ramVertices[2*i] = ramVertices[2*i] + m_Transform.translation.x;
                ramVertices[2*i + 1] = ramVertices[2*i + 1] + m_Transform.translation.y;
            }
            m_Batcher->AddQuad(ramVertices, it->GetWorldUVs());
        }
        else {
            m_Batcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs());
        }

        // 彙整到批次渲染器中 (假設您的 batcher 吃頂點與顏色) [6]

        ++it;
    }

    m_Batcher->EndBatch(); // 結束收集，交由 Renderer 自動呼叫 Draw()
}