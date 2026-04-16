//
// Created by jerry on 2026/3/20.
//

#include "Obstacle.hpp"

void Obstacle::UpdateStateByBeat(float currentBeat, glm::vec2 PlayerPos) {
    if (currentBeat > m_Event.EndBeat) {
        m_IsDead = true; // 標記為可銷毀
        return;
    }

    if (this->customBehavior != nullptr) {
        customBehavior(*this, currentBeat, PlayerPos);
    }
}

void Obstacle::UpdateWorldVertices() {
    // 每次更新前清空上一幀的資料，並預留足夠的空間以優化效能
    m_WorldVertices.clear();
    m_WorldVertices.reserve(m_LocalVertices.size());

    // 1. 提取 Transform 的數值，並預先算出 sin 與 cos (只算一次，節省效能)
    float scaleX = m_Transform.scale.x;
    float scaleY = m_Transform.scale.y;
    float transX = m_Transform.translation.x;
    float transY = m_Transform.translation.y;

    // 框架的 rotation 單位是弧度(rad)，直接丟入三角函數即可
    float cosTheta = std::cos(m_Transform.rotation);
    float sinTheta = std::sin(m_Transform.rotation);

    // 2. 遍歷 m_LocalVertices 中的每一個頂點 (每 2 個 float 是一組 X, Y)
    for (size_t i = 0; i < m_LocalVertices.size(); i += 2) {
        float localX = m_LocalVertices[i];
        float localY = m_LocalVertices[i + 1];

        // 步驟 A：縮放 (Scale)
        // 先把 1x1 的形狀拉伸成我們設定的寬高
        float scaledX = localX * scaleX;
        float scaledY = localY * scaleY;

        // 步驟 B：旋轉 (Rotate)
        // 套用 2D 旋轉矩陣公式
        float rotatedX = scaledX * cosTheta - scaledY * sinTheta;
        float rotatedY = scaledX * sinTheta + scaledY * cosTheta;

        // 步驟 C：位移 (Translate)
        // 最後加上物件在遊戲世界中的實際位置
        float worldX = rotatedX + transX;
        float worldY = rotatedY + transY;

        // 將算好的絕對座標存入 worldVertices
        m_WorldVertices.push_back(worldX);
        m_WorldVertices.push_back(worldY);
    }
    //m_WorldUVs = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f};
}

bool Obstacle::CheckCollision(glm::vec2 PlayerPos) const {
    // ==========================================
    // 第一階段：粗略檢查 (Broad Phase)
    // ==========================================
    // 這裡我們取 scale 的最大值當作外接圓半徑的粗估值
    float maxRadius = std::max(m_Transform.scale.x, m_Transform.scale.y) * 1.5f;
    float dx = PlayerPos.x - m_Transform.translation.x;
    float dy = PlayerPos.y - m_Transform.translation.y;

    // 若距離平方大於半徑平方，代表玩家離這顆子彈還很遠
    if ((dx * dx + dy * dy) > (maxRadius * maxRadius)) {
        return false;
    }


    // ==========================================
    // 第二階段：精確檢查 (Narrow Phase - 邊緣叉積判定)
    // ==========================================
    // 因為 worldVertices 是一維陣列 [x1, y1, x2, y2...]，頂點數為 size / 2
    size_t vertexCount = m_WorldVertices.size() / 2;

    if (vertexCount < 3) return false; // 形狀不完整無法判定

    bool hasPositive = false;
    bool hasNegative = false;

    for (size_t i = 0; i < vertexCount; ++i) {
        // 取得當前邊緣的「起點」
        float startX = m_WorldVertices[i * 2];
        float startY = m_WorldVertices[i * 2 + 1];

        // 取得當前邊緣的「終點」(如果是最後一個點，終點就是第一個點，形成封閉迴圈)
        size_t nextIdx = (i + 1) % vertexCount;
        float endX = m_WorldVertices[nextIdx * 2];
        float endY = m_WorldVertices[nextIdx * 2 + 1];

        // 計算「邊緣向量 (Edge Vector)」
        float edgeX = endX - startX;
        float edgeY = endY - startY;

        // 計算「起點到玩家的向量 (Player Vector)」
        float toPlayerX = PlayerPos.x - startX;
        float toPlayerY = PlayerPos.y - startY;

        // 2D 叉積 (Cross Product) 公式：判斷左右側
        float crossProduct = (edgeX * toPlayerY) - (edgeY * toPlayerX);

        // 紀錄叉積的正負號
        if (crossProduct > 0) hasPositive = true;
        if (crossProduct < 0) hasNegative = true;

        // 如果同時出現正號與負號，代表玩家在某些邊的左側、某些邊的右側，
        // 意味著玩家絕對不在多邊形「內部」，可以直接提早 return false！
        if (hasPositive && hasNegative) {
            return false;
        }
    }
    // 如果全部檢查完，符號都完全一致，代表玩家完美落入子彈內部！
    return true;
};

bool Obstacle::CheckCircleCollision(glm::vec2 PlayerPos) const {

    float dx = PlayerPos.x - m_Transform.translation.x;
    float dy = PlayerPos.y - m_Transform.translation.y;

    float circleRadius = m_Transform.translation.x;
    float circleDistance = glm::length(glm::vec2(dx, dy));

    if (circleDistance <= circleRadius) {
        return true;
    }
    if (to_int(m_Event.ShapeType) == 4) {
        float i = dx * glm::cos(m_Transform.rotation) - dy * glm::sin(m_Transform.rotation);
        float j = dx * glm::sin(m_Transform.rotation) + dy * glm::cos(m_Transform.rotation);
        float a = glm::max(glm::abs(i), glm::abs(j));
        float b = glm::min(glm::abs(i), glm::abs(j));
        float diamondDist = 1.55 * a - 0.45 * b;
        if (diamondDist <= m_Transform.scale.x) {
            return true;
        }
    }
    return false;
}
