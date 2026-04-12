//
// Created by jerry on 2026/4/9.
//
#include "BatchedColorShape.hpp"

BatchedColorShape::BatchedColorShape(const Util::Color &color) :m_Color(color){
    m_Program = std::make_unique<Core::Program>("../PTSD/assets/shaders/Base.vert", "../PTSD/assets/shaders/Base.frag");
    m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(*m_Program, "Transform", 0);

    // 2. 生成 1x1 純色紋理 (只需要做一次)
    Uint8 data[] = { static_cast<Uint8>(m_Color.r), static_cast<Uint8>(m_Color.g), static_cast<Uint8>(m_Color.b), static_cast<Uint8>(m_Color.a), 255, 255, 255, 255 };
    m_Texture = std::make_unique<Core::Texture>(GL_RGBA, 2, 1, data);
}

void BatchedColorShape::BeginBatch() {
    m_Positions.clear();
    m_UVs.clear();
    m_Indices.clear();
}

void BatchedColorShape::AddQuad(const std::vector<float> &worldVertices, const std::vector<float> &worldUVs) {
    if (worldVertices.empty()) return;

    unsigned int offset = m_Positions.size() / 2;

    m_Positions.insert(m_Positions.end(), worldVertices.begin(), worldVertices.end());
    m_UVs.insert(m_UVs.end(), worldUVs.begin(), worldUVs.end());
    for (int i = 0; i < static_cast<int>(worldVertices.size()/4); ++i) {
        //indices.push_back(i);
        m_Indices.push_back(offset + 0);
        m_Indices.push_back(offset + i + 1);
        m_Indices.push_back(offset + i + 2);
    }
}

void BatchedColorShape::EndBatch() {
    if (m_Positions.empty()) return;

    // 當 std::make_unique 覆寫舊指標時，框架底層會自動觸發 glDeleteBuffers 清理顯示卡記憶體
    m_VertexArray = std::make_unique<Core::VertexArray>();

    // 利用框架現有的建構子，把收集好的龐大陣列一次塞進去
    m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(m_Positions, 2));
    m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(m_UVs, 2));
    m_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(m_Indices));
}

void BatchedColorShape::Draw(const Core::Matrices &data) {
    LOG_DEBUG("start draw");
    // 如果這幀沒有任何東西要畫，或者 VertexArray 還沒建好，就直接跳過
    if (m_Positions.empty() || !m_VertexArray) return;

    Core::Matrices identityMatrix = { glm::mat4(1.0f), data.m_Projection };

    m_Program->Bind();
    m_UniformBuffer->SetData(0, identityMatrix);
    m_Texture->Bind(0);
    m_VertexArray->Bind();
    m_VertexArray->DrawTriangles();
}
