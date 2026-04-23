//
// Created by jerry on 2026/3/20.
//

#include "LevelSpawner.hpp"

void LevelSpawner::Start(float StartBeat) {
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
        if (item["StartBeat"]  < StartBeat) {
            continue;
        }
        SpawnEvent m_LoadEvent;
        //資料進來後根據障礙種類去做分類
        if (item["ObstacleType"] == "RotatingRectangle") {//瞬發
            m_LoadEvent.Bullet = BulletType::RotatingRectangle;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 14.0f;
            m_LoadEvent.StartPos = {item["StartPos"]["X"], item["StartPos"]["Y"]};
            m_LoadEvent.StartRot = 0.0f;
            m_LoadEvent.SpecialData.Velocity = 180.0f;
            m_LoadEvent.SpecialData.AngularVelocity = glm::pi<float>();
            m_LoadEvent.Scale = glm::vec2{15.0f, 15.0f};
            m_LoadEvent.DrawID = 3;
        }
        else if (item["ObstacleType"] == "Laser") {//4拍預備，1拍執行
            m_LoadEvent.Bullet = BulletType::Laser;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 4.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 5.0f;
            m_LoadEvent.StartPos = {WINDOW_WIDTH / 2, item["Offset"]};
            m_LoadEvent.StartRot = item["StartRotation"];
        }
        else if (item["ObstacleType"] == "SpawnerTriangle") {//4拍移動後射球，4拍持續射球
            m_LoadEvent.Bullet = BulletType::SpawnerTriangle;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 4.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 8.0f;
            m_LoadEvent.StartPos = {WINDOW_WIDTH / 2 + 300, item["PausePos"]["Y"]};
            m_LoadEvent.StartRot = 0.0f;
            m_LoadEvent.SpecialData.PausePos = {item["PausePos"]["X"], item["PausePos"]["Y"]};
            m_LoadEvent.EndPos = {300.0f - static_cast<float>(WINDOW_WIDTH) / 2, item["PausePos"]["Y"]};
            m_LoadEvent.Scale = {50.0f, 60.0f};
            m_LoadEvent.SpecialData.FireCount = 0;
        }
        else if (item["ObstacleType"] == "SpikeBall") {//1拍定位，1拍停留後炸裂，
            m_LoadEvent.Bullet = BulletType::SpikeBall;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 2.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 2.0f;
            m_LoadEvent.SpecialData.AngularVelocity = 3.14;
            m_LoadEvent.SpecialData.FireCount = 0;
        }
        else if (item["ObstacleType"] == "BiggerSpikeBall") {//1拍定位，10拍放大後炸裂
            m_LoadEvent.Bullet = BulletType::BiggerSpikeBall;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 11.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 11.0f;
            m_LoadEvent.SpecialData.AngularVelocity = 3.14;
            m_LoadEvent.SpecialData.FireCount = 0;
        }
        else if (item["ObstacleType"] == "BiggerLaser") {//3拍預告後撞擊，0.25拍持續撞擊，1.25拍停留，1拍收回
            m_LoadEvent.Bullet = BulletType::BiggerLaser;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 3.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 5.5f;
            m_LoadEvent.Scale = {0.0f, 150.0f};
        }
        else if (item["ObstacleType"] == "ExpendingBall") {//4拍預告後放大，4拍持續放大，0.5拍縮小
            m_LoadEvent.Bullet = BulletType::ExpendingBall;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.SpecialData.SpawnBeat = static_cast<float>(item["StartBeat"]) + 4.0f;
            m_LoadEvent.EndBeat = static_cast<float>(item["StartBeat"]) + 8.5f;
            m_LoadEvent.Scale = {200.0f, 200.0f};
        }
        else if (item["ObstacleType"] == "SpawnerRectangle") {
            m_LoadEvent.Bullet = BulletType::SpawnerRectangle;
            m_LoadEvent.StartBeat = item["StartBeat"];
            m_LoadEvent.EndBeat = item["EndBeat"];
            m_LoadEvent.StartRot = 0.0f;
            m_LoadEvent.SpecialData.Velocity = 180.0f;
            m_LoadEvent.SpecialData.AngularVelocity = glm::pi<float>();
            m_LoadEvent.Scale = glm::vec2{15.0f, 15.0f};
        }

        m_PendingEvents.push(m_LoadEvent);
    }
    m_ActiveObstacles.resize(2000);
    LOG_DEBUG("finishedbuild");

    m_CircleBatcher->SetDrawID(3);
    m_SpikeBatcher->SetDrawID(4);
    m_DottedCircleBatcher->SetDrawID(5);

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
    m_SpikeBatcher->BeginBatch();
    m_DottedCircleBatcher->BeginBatch();
    // 2. 更新所有存活的障礙物狀態，並清理過期的障礙物
    for (auto it = m_ActiveObstacles.begin(); it != m_ActiveObstacles.end(); ) {

        if (it->m_Event.StartBeat > currentBeat || it->m_Event.EndBeat < currentBeat || it->IsDead() || !it->IsActive()) {
            ++it;
            continue;
        }

        it->UpdateStateByBeat(currentBeat, PlayerPos);


        if (it->m_IsColliding) {
            m_IsColliding = true;
        }

        if (to_int(it->m_Event.Bullet) == to_int(BulletType::EasingBall) || to_int(it->m_Event.Bullet) == to_int(BulletType::EffectBall) || it->m_Event.Bullet == BulletType::ExpendingBall) {
            m_CircleBatcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs(), it->GetLocalVertices());
        }
        else if (to_int(it->m_Event.Bullet) == to_int(BulletType::SpikeBall) || to_int(it->m_Event.Bullet) == to_int(BulletType::BiggerSpikeBall)) {
            m_SpikeBatcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs(), it->GetLocalVertices());
        }
        else if (it->m_Event.Bullet == BulletType::WarningExpendingBall) {
            m_DottedCircleBatcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs(), it->GetLocalVertices());
        }
        else {
            m_Batcher->AddQuad(it->GetWorldVertices(), it->GetWorldUVs());
        }

        // 彙整到批次渲染器中 (假設您的 batcher 吃頂點與顏色) [6]

        ++it;
    }

    m_Batcher->EndBatch(); // 結束收集，交由 Renderer 自動呼叫 Draw()
    m_CircleBatcher->EndBatch();
    m_SpikeBatcher->EndBatch();
    m_DottedCircleBatcher->EndBatch();
}

void LevelSpawner::CreateObstacle(SpawnEvent m_SpawnEvent, glm::vec2 PlayerPos) {
    Obstacle* newObs = GetActiveObstacle();

    if (m_SpawnEvent.Bullet == BulletType::RotatingRectangle) {//旋轉方塊
        SpawnEvent CircleEvent = m_SpawnEvent;
        SpawnEvent RectangleEvent = m_SpawnEvent;

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
            RectangleEvent.SpecialData.UnitVector = glm::vec2{glm::cos(thetas[i]), glm::sin(thetas[i])};
            RectangleEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * velocityWeight;

            newObs = GetActiveObstacle();

            newObs->customBehavior = [](Obstacle& self, float beat, glm::vec2 PlayerPos) {

                self.m_Transform.rotation = beat * self.m_Event.SpecialData.AngularVelocity;

                self.m_Transform.translation = self.m_Event.StartPos +
                    glm::vec2{self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.x * (beat - self.m_Event.StartBeat),
                        self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.y * (beat - self.m_Event.StartBeat)};

                self.UpdateWorldVertices();

                self.m_IsColliding = self.CheckCollision(PlayerPos);
            };
            newObs->Spawn(RectangleEvent, m_SpawnVertices);
            //起始跟隨方塊的圓形

            CircleEvent.Bullet = BulletType::EffectBall;
            CircleEvent.SpecialData.UnitVector = glm::vec2{glm::cos(thetas[i]), glm::sin(thetas[i])};
            CircleEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * velocityWeight;
            CircleEvent.EndBeat = CircleEvent.StartBeat + 1;

            CreateObstacle(CircleEvent, PlayerPos);
        }
    }
    else if (m_SpawnEvent.Bullet == BulletType::SpawnerRectangle) {

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        SpawnEvent RectangleEvent = m_SpawnEvent;

        std::uniform_int_distribution<int> chance(0, 3);
        std::uniform_real_distribution<float> PosY(-(static_cast<float>(WINDOW_HEIGHT) / 2) + 200, static_cast<float>(WINDOW_HEIGHT) / 2 - 200);
        std::uniform_real_distribution<float> dis(0.0f, 0.7854f);
        std::uniform_real_distribution<float> val(0.8f, 1.2f);
        float i = static_cast<float>(m_SpawnEvent.StartBeat);
        while (i < static_cast<float>(m_SpawnEvent.StartBeat) + 16.0f) {
            i += 0.25f;
            if (chance(g)) {
                newObs = GetActiveObstacle();

                float theta = dis(g);
                float YIndex = PosY(g);
                if (YIndex < 0) {
                    RectangleEvent.SpecialData.UnitVector = glm::vec2{glm::cos(glm::pi<float>() - theta), glm::sin(glm::pi<float>() - theta)};
                }else {
                    RectangleEvent.SpecialData.UnitVector = glm::vec2{glm::cos(glm::pi<float>() + theta), glm::sin(glm::pi<float>() + theta)};
                }
                RectangleEvent.StartPos = {757.0f, YIndex};
                RectangleEvent.SpecialData.Velocity = m_SpawnEvent.SpecialData.Velocity * val(g);
                RectangleEvent.StartBeat = i;

                SpawnEvent CircleEvent = RectangleEvent;
                RectangleEvent.EndBeat = i + 14.0f;

                newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
                    self.m_Transform.rotation = beat * self.m_Event.SpecialData.AngularVelocity;

                    self.m_Transform.translation = self.m_Event.StartPos +
                        glm::vec2{self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.x * (beat - self.m_Event.StartBeat),
                            self.m_Event.SpecialData.Velocity * self.m_Event.SpecialData.UnitVector.y * (beat - self.m_Event.StartBeat)};

                    self.UpdateWorldVertices();

                    self.m_IsColliding = self.CheckCollision(PlayerPos);
                };

                newObs->Spawn(RectangleEvent, m_SpawnVertices);

                CircleEvent.Bullet = BulletType::EffectBall;
                CircleEvent.EndBeat = CircleEvent.StartBeat + 1;

                CreateObstacle(CircleEvent, PlayerPos);
            }
        }
    }
    else if (m_SpawnEvent.Bullet == BulletType::Laser) {//小型雷射
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        if ((m_SpawnEvent.StartRot >= 3.0f && m_SpawnEvent.StartRot <= 3.2f)) {
            m_SpawnEvent.StartPos = {-static_cast<float>(WINDOW_WIDTH) / 2, m_SpawnEvent.StartPos.y};
        }
        else if ((m_SpawnEvent.StartRot >= 1.5f && m_SpawnEvent.StartRot <= 1.6f)) {
            m_SpawnEvent.StartPos = {m_SpawnEvent.StartPos.y, -static_cast<float>(WINDOW_WIDTH) / 2};
        }
        else if ((m_SpawnEvent.StartRot >= 4.6f && m_SpawnEvent.StartRot <= 4.8f)) {
            m_SpawnEvent.StartPos = {m_SpawnEvent.StartPos.y, static_cast<float>(WINDOW_WIDTH) / 2};
        }

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float m_DetlaBeat = beat - self.GetLastBeat();
            float m_GapBeat = glm::abs(self.m_Event.SpecialData.SpawnBeat - beat);
            // 覆寫 X 軸位移，以原設定的 X 軸為基準，加上 Sin 波形
            if (beat <self.m_Event.SpecialData.SpawnBeat) {

                self.m_Transform.scale = {4000, 20 * (4 - m_GapBeat) / 4};
                float m_ColorValue = 0.25f + std::fmod(beat / 4, 0.15f);
                std::vector<float> warningUvs = {m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16, m_ColorValue, 0.7f - (4 - m_GapBeat) / 16};
                self.SetUvs(warningUvs);
            }
            else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < (self.m_Event.SpecialData.SpawnBeat + 0.25)) {
                self.TurnOnCollidable();
                if (!self.IsShaked()) {
                    this->VisionShake({-100 * glm::cos(self.m_Event.StartRot), -100 * glm::sin(self.m_Event.StartRot)}, self.m_Event.SpecialData.SpawnBeat);
                    self.HasShaked();
                }

                std::vector<float> Uvs = {0.75f, 0.25f, 0.75f, 0.25f, 0.75f, 0.25f, 0.75f, 0.25f};
                self.SetUvs(Uvs);
                self.m_Transform.scale = {4000 * (beat - self.m_Event.SpecialData.SpawnBeat) * 4, 20 * (beat - self.m_Event.SpecialData.SpawnBeat) * 4};
            }else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < (self.m_Event.SpecialData.SpawnBeat + 0.5)) {
                float m_TransferColor = 0.75f - ((m_GapBeat - 0.25f) * 2);
                std::vector<float> Uvs = {m_TransferColor, 0.25f, m_TransferColor, 0.25f, m_TransferColor, 0.25f, m_TransferColor, 0.25f};
                self.SetUvs(Uvs);
                self.m_Transform.scale = {4000, 20};
            }else if ( beat >= (self.m_Event.SpecialData.SpawnBeat + 0.75) && beat < (self.m_Event.SpecialData.SpawnBeat + 1)){

                std::vector<float> Uvs = {0.25f, m_GapBeat * 2 - 1.25f, 0.25f, m_GapBeat - 0.25f, 0.25f, m_GapBeat - 0.25f, 0.25f, m_GapBeat - 0.25f};
                self.SetUvs(Uvs);
                self.m_Transform.scale = {4000, 20 * (1 - m_GapBeat) * 4};
            }
            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCollision(PlayerPos);
        };
        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
        newObs->TurnOffCollidable();
    }
    else if (m_SpawnEvent.Bullet == BulletType::BiggerLaser) {

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        m_SpawnEvent.Scale = {0.0f, 150.0f};

        std::uniform_int_distribution<int> Rot(0, 3);
        std::uniform_real_distribution<float> PosX(-(static_cast<float>(WINDOW_WIDTH) / 2) + 200, static_cast<float>(WINDOW_WIDTH) / 2 - 200);
        std::uniform_real_distribution<float> PosY(-(static_cast<float>(WINDOW_HEIGHT) / 2) + 200, static_cast<float>(WINDOW_HEIGHT) / 2 - 200);

        int rotation = Rot(g);
        m_SpawnEvent.StartRot = static_cast<float>(rotation) * glm::pi<float>() / 2.0f;

        switch (rotation) {
            case 0:
                m_SpawnEvent.StartPos = glm::vec2{(static_cast<float>(WINDOW_WIDTH) / 2) - 200.0f, PosY(g)};
                break;
            case 1:
                m_SpawnEvent.StartPos = glm::vec2{PosX(g), static_cast<float>(WINDOW_HEIGHT) / 2 - 100.0f};
                break;
            case 2:
                m_SpawnEvent.StartPos = glm::vec2{-(static_cast<float>(WINDOW_WIDTH) / 2) + 200.0f, PosY(g)};
                break;
            case 3:
                m_SpawnEvent.StartPos = glm::vec2{PosX(g), -static_cast<float>(WINDOW_HEIGHT) / 2 + 100.0f};
                break;
        }

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float m_GapBeat = glm::abs(self.m_Event.SpecialData.SpawnBeat - beat);
            float movement = glm::sin( (m_GapBeat) * 25.5f * glm::pi<float>());

            if (beat <self.m_Event.SpecialData.SpawnBeat) {
                self.m_Transform.scale = {(3 - m_GapBeat) * 50, self.m_Event.Scale.y};
            }
            else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < (self.m_Event.SpecialData.SpawnBeat + 0.25)) {
                float m_ColorValue = 0.75f - std::fmod(m_GapBeat * 2, 0.5f);
                if (!self.IsShaked()) {
                    this->VisionShake({-100 * glm::cos(self.m_Event.StartRot), -100 * glm::sin(self.m_Event.StartRot)}, self.m_Event.SpecialData.SpawnBeat);
                    self.HasShaked();
                }
                std::vector<float> Uvs = {m_ColorValue, 0.25f, m_ColorValue, 0.25f, m_ColorValue, 0.25f, m_ColorValue, 0.25f};
                self.SetUvs(Uvs);
                if (self.m_Event.StartRot == 0.0f || (self.m_Event.StartRot >= 3.0f && self.m_Event.StartRot <= 3.2f)) {
                    self.m_Transform.translation = {self.m_Transform.translation.x, self.m_Event.StartPos.y + movement * 3};
                }
                else if ((self.m_Event.StartRot >= 1.5f && self.m_Event.StartRot <= 1.6f) || (self.m_Event.StartRot >= 4.65f && self.m_Event.StartRot <= 4.8f)) {
                    self.m_Transform.translation = {self.m_Event.StartPos.x + movement * 3, self.m_Transform.translation.y};
                }
                self.m_Transform.scale = {4000 * (beat - self.m_Event.SpecialData.SpawnBeat) * 4, self.m_Event.Scale.y};
            }else if (beat >= (self.m_Event.SpecialData.SpawnBeat + 0.25f) && beat < (self.m_Event.SpecialData.SpawnBeat + 1.5f)) {
                if (self.m_Event.StartRot == 0.0f || (self.m_Event.StartRot >= 3.0f && self.m_Event.StartRot <= 3.2f)) {
                    self.m_Transform.translation = {self.m_Transform.translation.x, self.m_Event.StartPos.y + movement * 3};
                }
                else if ((self.m_Event.StartRot >= 1.5f && self.m_Event.StartRot <= 1.6f) || (self.m_Event.StartRot >= 4.65f && self.m_Event.StartRot <= 4.8f)) {
                    self.m_Transform.translation = {self.m_Event.StartPos.x + movement * 3, self.m_Transform.translation.y};
                }
                self.m_Transform.scale = {4000, self.m_Event.Scale.y};
            }else if ( beat >= (self.m_Event.SpecialData.SpawnBeat + 1.5f) && beat < (self.m_Event.SpecialData.SpawnBeat + 2.5f)){
                self.m_Transform.scale = {4000 * (2 - (beat - self.m_Event.SpecialData.SpawnBeat - 0.5f)), self.m_Event.Scale.y};
            }
            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCollision(PlayerPos);
        };
        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent WarningShape;
        WarningShape.StartBeat = m_SpawnEvent.StartBeat;
        WarningShape.EndBeat = m_SpawnEvent.SpecialData.SpawnBeat;
        WarningShape.StartPos = m_SpawnEvent.StartPos;
        WarningShape.StartRot = m_SpawnEvent.StartRot;
        WarningShape.Bullet = BulletType::WarningBiggerLaser;

        CreateObstacle(WarningShape, PlayerPos);
    }
    else if (m_SpawnEvent.Bullet == BulletType::WarningBiggerLaser) {
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        m_SpawnEvent.Scale = glm::vec2{4000.0f, 150.0f};

        newObs->customBehavior = [](Obstacle& self, float beat, glm::vec2 PlayerPos) {

            std::vector<float> Uvs = {0.25f, 0.7f, 0.25f, 0.7f, 0.25f, 0.7f, 0.25f, 0.7f};
            self.SetUvs(Uvs);

            self.UpdateWorldVertices();
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
        newObs->TurnOffCollidable();
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
        newObs->TurnOffCollidable();
    }
    else if (m_SpawnEvent.Bullet == BulletType::SpawnerTriangle) {
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.3f, 0.0f};
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

            self.m_IsColliding = self.CheckCollision(PlayerPos);
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent BallEvent;
        BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x, 0.0f};
        BallEvent.Bullet = BulletType::EasingBall;
        BallEvent.SpecialData.Velocity = 450.0f;
        BallEvent.SpecialData.UnitVector = glm::vec2{glm::cos(m_SpawnEvent.StartRot), glm::sin(m_SpawnEvent.StartRot)};
        BallEvent.StartRot = m_SpawnEvent.StartRot;

        for (int i = 0; i < 7; i++) {
            BallEvent.StartBeat = m_SpawnEvent.SpecialData.SpawnBeat + i * 0.25f;
            BallEvent.EndBeat = m_SpawnEvent.SpecialData.SpawnBeat + i * 0.25f + 4.0f;
            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, 0.0f};
            this->CreateObstacle(BallEvent, PlayerPos);

            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, m_SpawnEvent.Scale.y * 0.6};
            this->CreateObstacle(BallEvent, PlayerPos);

            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - glm::vec2{m_SpawnEvent.Scale.x * 0.4, -m_SpawnEvent.Scale.y * 0.6};
            this->CreateObstacle(BallEvent, PlayerPos);
        }

    }
    else if (m_SpawnEvent.Bullet == BulletType::EasingBall) {

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        m_SpawnEvent.Scale = {15.0f, 15.0f};
        m_SpawnEvent.Bullet = BulletType::EasingBall;

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float Progress = (beat - self.m_Event.StartBeat);
            float movementX = Progress * self.m_Event.SpecialData.Velocity;
            float movementY = glm::sin( (movementX / 400) * 2 * glm::pi<float>());
            self.m_Transform.translation = self.m_Event.StartPos - glm::vec2{(glm::cos(self.m_Event.StartRot) * movementX + glm::sin(self.m_Event.StartRot) * movementY * 5), (glm::sin(self.m_Event.StartRot) * movementX + glm::cos(self.m_Event.StartRot) * movementY * 5)};

            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCircleCollision(PlayerPos);
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
    }
    else if (m_SpawnEvent.Bullet == BulletType::SpikeBall) {

        std::uniform_real_distribution<float> PosX(200, 500);
        std::uniform_real_distribution<float> PosY(-(static_cast<float>(WINDOW_HEIGHT) / 2) + 200, static_cast<float>(WINDOW_HEIGHT) / 2 - 200);

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        m_SpawnEvent.Scale = {50.0f, 50.0f};
        m_SpawnEvent.StartPos = {static_cast<float>(WINDOW_WIDTH) / 2 + 100, PosY(g)};
        m_SpawnEvent.SpecialData.PausePos = glm::vec2{PosX(g), PosY(g)};

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float Progress = (beat - self.m_Event.StartBeat);
            float UvTrans = std::fmod(Progress * 8, 0.5f);
            std::vector<float> Uvs = {0.25f + UvTrans, 0.25f, 0.25f + UvTrans, 0.25f, 0.25f + UvTrans, 0.25f, 0.25f + UvTrans, 0.25f};
            if (beat > self.m_Event.StartBeat && Progress < 1) {
                self.m_Transform.translation = {glm::mix(self.m_Event.StartPos.x, self.m_Event.SpecialData.PausePos.x, glm::pow(Progress,0.25)), glm::mix(self.m_Event.StartPos.y, self.m_Event.SpecialData.PausePos.y, glm::pow(Progress,0.25))};
                self.m_Transform.scale = {glm::mix(0.0f, 30.0f, Progress), glm::mix(0.0f, 30.0f, Progress)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
                self.SetUvs(Uvs);
            }
            else if (beat > self.m_Event.StartBeat + 1.0f && Progress <= 2.0f) {
                self.m_Transform.scale = {glm::mix(30.0f, self.m_Event.Scale.x, Progress - 1), glm::mix(30.0f, self.m_Event.Scale.y, Progress - 1)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
                self.SetUvs(Uvs);
                if (self.m_Event.SpecialData.FireCount == 0) {
                    self.m_Event.SpecialData.FireCount += 8;
                }
            }
            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCircleCollision(PlayerPos);
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent BallEvent;
        BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos;
        BallEvent.Bullet = BulletType::EasingBall;
        BallEvent.SpecialData.Velocity = 450.0f;
        BallEvent.StartBeat = m_SpawnEvent.SpecialData.SpawnBeat;
        BallEvent.EndBeat = m_SpawnEvent.SpecialData.SpawnBeat + 5.0f;

        for (int i = 0; i < 8; i++) {
            BallEvent.SpecialData.UnitVector = glm::vec2{glm::cos(static_cast<float>(i) * glm::pi<float>() / 4.0f), glm::sin(static_cast<float>(i) * glm::pi<float>() / 4.0f)};
            BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - BallEvent.SpecialData.UnitVector * glm::vec2{5.0f, 5.0f};
            BallEvent.StartRot = static_cast<float>(i) * glm::pi<float>() / 4.0f;
            this->CreateObstacle(BallEvent, PlayerPos);
        }
    }
    else if (m_SpawnEvent.Bullet == BulletType::BiggerSpikeBall) {

        std::uniform_real_distribution<float> PosY(-(static_cast<float>(WINDOW_HEIGHT) / 2) + 200, static_cast<float>(WINDOW_HEIGHT) / 2 - 200);

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        m_SpawnEvent.Scale = {200.0f, 200.0f};
        m_SpawnEvent.StartPos = {static_cast<float>(WINDOW_WIDTH) / 2 + 100, PosY(g)};
        m_SpawnEvent.SpecialData.PausePos = glm::vec2{150.0f, 0.0f};

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {
            float Progress = (beat - self.m_Event.StartBeat);
            float UvTrans = std::fmod(Progress / 2 + 0.25f, 0.5f);
            std::vector<float> Uvs;
            if (beat > self.m_Event.StartBeat && Progress < 1) {
                self.m_Transform.translation = {glm::mix(self.m_Event.StartPos.x, self.m_Event.SpecialData.PausePos.x, glm::pow(Progress,0.25)), glm::mix(self.m_Event.StartPos.y, self.m_Event.SpecialData.PausePos.y, glm::pow(Progress,0.25))};
                self.m_Transform.scale = {glm::mix(0.0f, 30.0f, Progress), glm::mix(0.0f, 30.0f, Progress)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
            }
            else if (beat > self.m_Event.StartBeat + 1.0f && Progress <= 3.0f) {
                self.m_Transform.scale = {glm::mix(30.0f, self.m_Event.Scale.x, (Progress - 1) / 10), glm::mix(30.0f, self.m_Event.Scale.y, (Progress - 1) / 10)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
            }
            else if (beat > self.m_Event.StartBeat + 2.0f && Progress <= 7.0f) {
                self.m_Transform.scale = {glm::mix(30.0f, self.m_Event.Scale.x, (Progress - 1) / 10), glm::mix(30.0f, self.m_Event.Scale.y, (Progress - 1) / 10)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
                Uvs = {0.75f - UvTrans, 0.25f, 0.75f - UvTrans, 0.25f, 0.75f - UvTrans, 0.25f, 0.75f - UvTrans, 0.25f};
                self.SetUvs(Uvs);
            }
            else if (beat > self.m_Event.StartBeat + 6.0f && Progress <= 10.5f) {
                float UvTrans2 = std::fmod(Progress, 0.5f);
                self.m_Transform.scale = {glm::mix(30.0f, self.m_Event.Scale.x, (Progress - 1) / 10), glm::mix(30.0f, self.m_Event.Scale.y, (Progress - 1) / 10)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity;
                Uvs = {0.75f - UvTrans2, 0.25f, 0.75f - UvTrans2, 0.25f, 0.75f - UvTrans2, 0.25f, 0.75f - UvTrans2, 0.25f};
                self.SetUvs(Uvs);
            }
            else if (beat > self.m_Event.StartBeat + 10.5f && Progress <= 11.0f) {
                float UvTrans3 = std::fmod(Progress * 2, 0.5f);
                self.m_Transform.scale = {glm::mix(30.0f, self.m_Event.Scale.x, (Progress - 1) / 10), glm::mix(30.0f, self.m_Event.Scale.y, (Progress - 1) / 10)};
                self.m_Transform.rotation = Progress * self.m_Event.SpecialData.AngularVelocity * 2;
                Uvs = {0.75f - UvTrans3, 0.25f, 0.75f - UvTrans3, 0.25f, 0.75f - UvTrans3, 0.25f, 0.75f - UvTrans3, 0.25f};
                self.SetUvs(Uvs);
            }

            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCircleCollision(PlayerPos);
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent BallEvent;
        BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos;
        BallEvent.Bullet = BulletType::EasingBall;
        BallEvent.SpecialData.Velocity = 450.0f;
        BallEvent.StartBeat = m_SpawnEvent.SpecialData.SpawnBeat;
        BallEvent.EndBeat = m_SpawnEvent.SpecialData.SpawnBeat + 11.0f;

        for (int j = 0; j < 11; j++) {
            BallEvent.StartBeat += 0.25f;
            BallEvent.EndBeat += 0.25f;
            for (int i = 0; i < 16; i++) {
                BallEvent.SpecialData.UnitVector = glm::vec2{glm::cos(static_cast<float>(i) * glm::pi<float>() / 8.0f + static_cast<float>(j) * glm::pi<float>() / 24.0f), glm::sin(static_cast<float>(i) * glm::pi<float>() / 8.0f + static_cast<float>(j) * glm::pi<float>() / 24.0f)};
                BallEvent.StartPos = m_SpawnEvent.SpecialData.PausePos - BallEvent.SpecialData.UnitVector * 10.0f;
                BallEvent.StartRot = static_cast<float>(i) * glm::pi<float>() / 8.0f + static_cast<float>(j) * glm::pi<float>() / 24.0f;
                this->CreateObstacle(BallEvent, PlayerPos);
            }
        }
    }
    else if (m_SpawnEvent.Bullet == BulletType::ExpendingBall) {
        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        std::uniform_real_distribution<float> PosX(-(static_cast<float>(WINDOW_WIDTH) / 2) + 200, static_cast<float>(WINDOW_WIDTH) / 2 - 200);
        std::uniform_real_distribution<float> PosY(-(static_cast<float>(WINDOW_HEIGHT) / 2) + 200, static_cast<float>(WINDOW_HEIGHT) / 2 - 200);
        m_SpawnEvent.StartPos = {PosX(g), PosY(g)};

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {

            float OriginScale = self.m_Event.Scale.x;
            float Progress = (beat - self.m_Event.StartBeat);
            float UvTrans = 0.25f + glm::abs(2 * std::fmod(Progress + 0.5f, 1.0f) - 1.0f) / 2.0f;
            std::vector<float> Uvs;

            if (beat >= self.m_Event.StartBeat && beat < self.m_Event.SpecialData.SpawnBeat) {
                self.m_Transform.scale = {0.0f, 0.0f};
            }
            else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < self.m_Event.SpecialData.SpawnBeat + 0.5f) {
                self.m_Transform.scale = {OriginScale * (Progress - 4) * 2, OriginScale * (Progress - 4) * 2};
                Uvs = {UvTrans, 0.25f, UvTrans, 0.25f, UvTrans, 0.25f, UvTrans, 0.25f};
                self.SetUvs(Uvs);
            }
            else if (beat >= self.m_Event.SpecialData.SpawnBeat && beat < self.m_Event.EndBeat - 0.5f) {
                self.m_Transform.scale = {  OriginScale+ OriginScale * (Progress - 4.5) / 4, OriginScale + OriginScale * (Progress - 4.5) / 4};
                Uvs = {UvTrans, 0.25f, UvTrans, 0.25f, UvTrans, 0.25f, UvTrans, 0.25f};
                self.SetUvs(Uvs);

            }
            else if (beat >= self.m_Event.EndBeat - 0.5f && beat < self.m_Event.EndBeat) {
                self.m_Transform.scale = { OriginScale * 2 * (8.5f - Progress) * 2, OriginScale * 2 * (8.5f - Progress) * 2};
            }

            self.UpdateWorldVertices();

            self.m_IsColliding = self.CheckCircleCollision(PlayerPos);
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);

        SpawnEvent WarningBall;
        WarningBall.StartPos = m_SpawnEvent.StartPos;
        WarningBall.Bullet = BulletType::WarningExpendingBall;
        WarningBall.StartBeat = m_SpawnEvent.StartBeat;
        WarningBall.EndBeat = m_SpawnEvent.EndBeat;
        WarningBall.Scale = m_SpawnEvent.Scale;

        CreateObstacle(WarningBall, PlayerPos);
    }
    else if (m_SpawnEvent.Bullet == BulletType::WarningExpendingBall) {

        m_SpawnVertices = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};

        newObs->customBehavior = [this](Obstacle& self, float beat, glm::vec2 PlayerPos) {

            float Progress = (beat - self.m_Event.StartBeat);
            if(beat >= self.m_Event.StartBeat){
                self.m_Transform.rotation = -Progress / 8 * glm::pi<float>();
            }
            
            self.UpdateWorldVertices();
        };

        newObs->Spawn(m_SpawnEvent, m_SpawnVertices);
        newObs->TurnOffCollidable();
    }

}

void LevelSpawner::DrawAll() {
    if (!m_Visible || m_Drawable == nullptr) {
        return;
    }

    this->SetZIndex(15);

    auto data = Util::ConvertToUniformBufferData(
        m_Transform, m_DottedCircleBatcher->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_DottedCircleBatcher->GetSize(), 0} * -1.0F);

    m_DottedCircleBatcher->Draw(data);

    this->SetZIndex(20);

    data = Util::ConvertToUniformBufferData(
        m_Transform, m_CircleBatcher->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_CircleBatcher->GetSize(), 0} * -1.0F);

    m_CircleBatcher->Draw(data);

    this->SetZIndex(25);

    data = Util::ConvertToUniformBufferData(
        m_Transform, m_SpikeBatcher->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_SpikeBatcher->GetSize(), 0} * -1.0F);

    m_SpikeBatcher->Draw(data);

    this->SetZIndex(30);

    data = Util::ConvertToUniformBufferData(
        m_Transform, m_Drawable->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_Drawable->GetSize(), 0} * -1.0F);

    m_Drawable->Draw(data);

}
