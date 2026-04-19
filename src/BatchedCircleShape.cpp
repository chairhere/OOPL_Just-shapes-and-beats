//
// Created by jerry on 2026/4/14.
//
#include "BatchedCircleShape.hpp"

BatchedCircleShape::BatchedCircleShape(const Util::Color &color) :m_Color(color){
    // 核心關鍵：在這裡載入您剛剛寫好的圓形 Shader！
    m_Program = std::make_unique<Core::Program>(
        "../Resources/shaders/Circle.vert",
        "../Resources/shaders/Circle.frag"
    );
    m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(*m_Program, "Matrices", 0);
    Uint8 data[] = { static_cast<Uint8>(m_Color.r), static_cast<Uint8>(m_Color.g), static_cast<Uint8>(m_Color.b), static_cast<Uint8>(m_Color.a), 255, 255, 255, 255 };
    m_Texture = std::make_unique<Core::Texture>(GL_RGBA, 2, 1, data);

    DrawID = 3;
}

void BatchedCircleShape::SetDrawID(const int drawID) {
    DrawID = drawID;
}

void BatchedCircleShape::BeginBatch() {
    m_Positions.clear();
    m_Vertices.clear();
    m_UVs.clear();
    m_Indices.clear();
}

void BatchedCircleShape::AddQuad(const std::vector<float> &worldVertices, const std::vector<float> &worldUVs, const std::vector<float>& localVertices) {
    if (worldVertices.empty()) return;

    unsigned int offset = m_Positions.size() / 2;

    m_Positions.insert(m_Positions.end(), worldVertices.begin(), worldVertices.end());
    m_Vertices.insert(m_Vertices.end(), localVertices.begin(), localVertices.end());
    m_UVs.insert(m_UVs.end(), worldUVs.begin(), worldUVs.end());
    for (int i = 0; i < static_cast<int>(worldVertices.size()/4); ++i) {
        //indices.push_back(i);
        m_Indices.push_back(offset + 0);
        m_Indices.push_back(offset + i + 1);
        m_Indices.push_back(offset + i + 2);
    }
}

void BatchedCircleShape::EndBatch() {
    if (m_Positions.empty()) return;

    // 當 std::make_unique 覆寫舊指標時，框架底層會自動觸發 glDeleteBuffers 清理顯示卡記憶體
    m_VertexArray = std::make_unique<Core::VertexArray>();

    // 利用框架現有的建構子，把收集好的龐大陣列一次塞進去
    m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(m_Positions, 2));
    m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(m_UVs, 2));
    m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(m_Vertices, 2));
    m_VertexArray->SetIndexBuffer(std::make_unique<Core::IndexBuffer>(m_Indices));
}

void BatchedCircleShape::Draw(const Core::Matrices &data){
    // 如果這幀沒有任何東西要畫，或者 VertexArray 還沒建好，就直接跳過
    if (m_Positions.empty() || !m_VertexArray) return;

    m_Program->Bind();

    // 更新矩陣 (框架底層機制)
    //m_UniformBuffer->SetData(0, data);

    GLint BulletShape = glGetUniformLocation(m_Program->GetId(), "BulletType");
    glUniform1i(BulletShape, DrawID);
    Core::Matrices identityMatrix = { glm::mat4(1.0f), data.m_Projection };

     //m_Program->Bind();
    m_UniformBuffer->SetData(0, data);
    m_Texture->Bind(0);//不影響
    m_VertexArray->Bind();//不影響
    // 畫出 Quad
    m_VertexArray->DrawTriangles();
}