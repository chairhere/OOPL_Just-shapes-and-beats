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
    LOG_DEBUG("startbuild");
    for (const auto& item : m_LevelData) {
        SpawnEvent m_LoadEvent;
        //資料進來後根據障礙種類去做分類
        if (item["ObstacleType"] == "RotatingRectangle") {
            m_LoadEvent.Bullet = BulletType::RotatingRectangle;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 14.0f;
            m_LoadEvent.StartPos = {item["StartPos"]["X"], item["StartPos"]["Y"]};
            m_LoadEvent.StartRot = 0.0f;
            m_LoadEvent.SpecialData.Velocity = 180.0f;
            m_LoadEvent.SpecialData.AngularVelocity = glm::pi<float>();
            m_LoadEvent.Scale = glm::vec2{15.0f, 15.0f};
        }
        else if (item["ObstacleType"] == "Laser") {
            m_LoadEvent.Bullet = BulletType::Laser;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = item["SpawnBeat"];
            m_LoadEvent.EndBeat = static_cast<float>(item["SpawnBeat"]) + 1.0f;
            m_LoadEvent.StartPos = {WINDOW_WIDTH / 2, item["YIndex"]};
            m_LoadEvent.StartRot = item["StartRotation"];
        }
        else if (item["ObstacleType"] == "SpawnerTriangle") {
            LOG_DEBUG("triangle");
            m_LoadEvent.Bullet = BulletType::SpawnerTriangle;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 4.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 8.0f;
            m_LoadEvent.StartPos = {WINDOW_WIDTH / 2 + 300, item["PausePos"]["Y"]};
            m_LoadEvent.StartRot = 0.0f;
            m_LoadEvent.SpecialData.PausePos = {item["PausePos"]["X"], item["PausePos"]["Y"]};
            m_LoadEvent.EndPos = {300.0f - static_cast<float>(WINDOW_WIDTH) / 2, item["PausePos"]["Y"]};
            m_LoadEvent.Scale = {50.0f, 50.0f};
            m_LoadEvent.SpecialData.FireCount = 0;
        }
        else if (item["ObstacleType"] == "SpikeBall") {

        }
        m_PendingEvents.push(m_LoadEvent);
    }
    m_ActiveObstacles.resize(2000);
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
        SpawnEvent Event;
        Event = m_PendingEvents.front();

        CreateObstacle(Event, PlayerPos);

        m_PendingEvents.pop(); // 移除已生成的事件
    }
    m_Batcher->BeginBatch();
    m_CircleBatcher->BeginBatch();
    // 2. 更新所有存活的障礙物狀態，並清理過期的障礙物
    for (auto it = m_ActiveObstacles.begin(); it != m_ActiveObstacles.end(); ) {

        if (it->m_Event.StartBeat > currentBeat || it->m_Event.EndBeat < currentBeat || it->IsDead() || !it->IsActive()) {
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

        if (it->m_IsColliding) {
            m_IsColliding = true;
        }

        if (to_int(it->m_Event.Bullet) >= to_int(BulletType::EasingBall) && to_int(it->m_Event.Bullet) <= to_int(BulletType::SpikeBall)) {
            m_CircleBatcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs(), it->GetLocalVertices());
        }
        else {
            m_Batcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs());
        }

        // 彙整到批次渲染器中 (假設您的 batcher 吃頂點與顏色) [6]

        ++it;
    }

    m_Batcher->EndBatch(); // 結束收集，交由 Renderer 自動呼叫 Draw()
    m_CircleBatcher->EndBatch();
}

void LevelSpawner::CreateObstacle(SpawnEvent m_SpawnEvent, glm::vec2 PlayerPos) {
    Obstacle* newObs = GetActiveObstacle();

    if (m_SpawnEvent.Bullet == BulletType::RotatingRectangle) {//旋轉方塊
        SpawnEvent CircleEvent = m_SpawnEvent;
        CircleEvent.Bullet = BulletType::EffectBall;

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        //取得障礙與玩家的角度並發射三枚(上、中、下)
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
            m_SpawnEvent.SpecialData.UnitVector = glm::vec2{glm::cos(thetas[i]), glm::sin(thetas[i])};
            m_SpawnEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * velocityWeight;

            newObs = GetActiveObstacle();

            newObs->customBehavior = [](Obstacle& self, float beat, glm::vec2 PlayerPos) {

                self.m_Transform.rotation = beat * self.m_Event.SpecialData.AngularVelocity;

                self.m_Transform.translation = self.m_Event.StartPos +
                    glm::vec2{self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.x * (beat - self.m_Event.StartBeat),
                        self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.y * (beat - self.m_Event.StartBeat)};

                self.UpdateWorldVertices();

                self.m_IsColliding = self.CheckCollision(PlayerPos);
            };
            newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
            //起始跟隨方塊的圓形

            CircleEvent.SpecialData.UnitVector = glm::vec2{glm::cos(thetas[i]), glm::sin(thetas[i])};
            CircleEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * velocityWeight;
            CircleEvent.EndBeat = CircleEvent.StartBeat + 1;

            CreateObstacle(CircleEvent, PlayerPos);
        }
    }
    else if (m_SpawnEvent.Bullet == BulletType::Laser) {//小型雷射
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

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
    else if (m_SpawnEvent.Bullet == BulletType::EffectBall) {//旋轉方塊的特效小球

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        m_SpawnEvent.Scale = {0.0f, 0.0f};

        newObs->customBehavior = [](Obstacle& self, float beat, glm::vec2 PlayerPos) {

            self.m_Transform.translation = self.m_Event.StartPos +
                glm::vec2{self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.x * (beat - self.m_Event.StartBeat),
                    self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.y * (beat - self.m_Event.StartBeat)};
            float Progress = (beat - self.m_Event.StartBeat);
            if (beat > self.m_Event.StartBeat && beat <= self.m_Event.StartBeat + 0.25) {
                self.m_Transform.scale = {60 * Progress * 4, 60 * Progress * 4};
                std::vector<float> Uvs = {Progress + 0.5f, 0.25f, Progress + 0.5f, 0.25f, Progress + 0.5f, 0.25f, Progress + 0.5f, 0.25f};
                self.SetUvs(Uvs);
            }
            else if (beat > self.m_Event.StartBeat && beat <= self.m_Event.StartBeat + 1){
                self.m_Transform.scale = {60 * glm::abs(beat - self.m_Event.StartBeat - 1), 60 * glm::abs(beat - self.m_Event.StartBeat - 1)};
                std::vector<float> Uvs = {(1 - Progress) * 2 / 3 + 0.25f, 0.25f, (1 - Progress) * 2 / 3 + 0.25f, 0.25f, (1 - Progress) * 2 / 3 + 0.25f, 0.25f, (1 - Progress) * 2 / 3 + 0.25f, 0.25f};
                self.SetUvs(Uvs);
            }

            self.UpdateWorldVertices();

        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
    }
    else if (m_SpawnEvent.Bullet == BulletType::SpawnerTriangle) {
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.35f, 0.0f};
        std::vector<float> Uvs = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f};


        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {

            float Progress = (beat - self.m_Event.StartBeat);
            if (beat > self.m_Event.StartBeat && beat <= self.m_Event.SpecialData.SpawnBeat) {
                float movement = glm::mix(0.0f,self.m_Event.StartPos.x - self.m_Event.SpecialData.PausePos.x , glm::pow(Progress/4, 0.25));
                self.m_Transform.translation = {self.m_Event.StartPos.x -  movement, self.m_Event.SpecialData.PausePos.y};
                std::vector<float> Uvs = {0.25f + Progress/8, 0.25f, 0.25f + Progress/8, 0.25f, 0.25f + Progress/8, 0.25f};
                self.SetUvs(Uvs);
            }
            else if (beat > self.m_Event.SpecialData.SpawnBeat && beat < self.m_Event.SpecialData.SpawnBeat + 1.75) {
                std::vector<float> Uvs = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f};
                self.SetUvs(Uvs);
                if (((beat - self.m_Event.SpecialData.SpawnBeat) / 0.25f) > (static_cast<float>(self.m_Event.SpecialData.FireCount))) {
                    self.m_Event.SpecialData.FireCount++;
                    self.m_Transform.translation = self.m_Event.SpecialData.PausePos + glm::vec2{10.0f, 0.0f};

                }
                else {
                    self.m_Transform.translation = self.m_Event.SpecialData.PausePos + glm::vec2{10.0f * (std::fmod(6 - Progress, 0.25f) * 4), 0.0f};
                }
            }
            else if (beat > self.m_Event.SpecialData.SpawnBeat + 1.75 && beat <= self.m_Event.EndBeat) {
                self.m_Transform.translation = {self.m_Event.SpecialData.PausePos.x  - glm::mix(0.0f,self.m_Event.SpecialData.PausePos.x - self.m_Event.EndPos.x , glm::pow((Progress - 5.75)/2.25, 3)), self.m_Event.SpecialData.PausePos.y};
            }

            self.UpdateWorldVertices();
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent BallEvent;
        BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x, 0.0f};
        BallEvent.Bullet = BulletType::EasingBall;
        BallEvent.SpecialData.Velocity = 450.0f;

        for (int i = 0; i < 7; i++) {
            BallEvent.StartBeat = m_SpawnEvent.SpecialData.SpawnBeat + i * 0.25f;
            BallEvent.EndBeat = m_SpawnEvent.SpecialData.SpawnBeat + i * 0.25f + 4.0f;
            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, 0.0f};
            this->CreateObstacle(BallEvent, PlayerPos);

            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, m_SpawnEvent.Scale.y * 0.45};
            this->CreateObstacle(BallEvent, PlayerPos);

            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, -m_SpawnEvent.Scale.y * 0.45};
            this->CreateObstacle(BallEvent, PlayerPos);
        }

    }
    else if (m_SpawnEvent.Bullet == BulletType::EasingBall) {

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        m_SpawnEvent.Scale = {10.0f, 10.0f};
        m_SpawnEvent.Bullet = BulletType::EasingBall;

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float Progress = (beat - self.m_Event.StartBeat);
            self.m_Transform.translation = self.m_Event.StartPos - glm::vec2{Progress * self.m_Event.SpecialData.Velocity, 5 * glm::sin(2 * Progress * glm::pi<float>())};

            self.UpdateWorldVertices();
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
    }
    else if (m_SpawnEvent.Bullet == BulletType::SpikeBall) {

    }

}

void LevelSpawner::DrawAll() {
    if (!m_Visible || m_Drawable == nullptr) {
        return;
    }
    this->SetZIndex(20);

    auto data = Util::ConvertToUniformBufferData(
        m_Transform, m_Drawable->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_Drawable->GetSize(), 0} * -1.0F);

    m_CircleBatcher->Draw(data);

    this->SetZIndex(30);

    data = Util::ConvertToUniformBufferData(
        m_Transform, m_Drawable->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_Drawable->GetSize(), 0} * -1.0F);

    m_Drawable->Draw(data);

}
