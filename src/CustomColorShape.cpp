//
// Created by jerry on 2026/3/26.
//
#include "CustomColorShape.hpp"

#include "spdlog/fmt/bundled/color.h"

CustomColorShape::CustomColorShape(const Util::Color& color, const std::vector<float> &Positions)
        : m_Color(color) {
        // 1. 定義頂點 (x, y, u, v)
        // 為了配合 Base.frag 讀取紋理，我們必須多傳入 u, v (紋理座標)
        std::vector<float> positions = Positions;

        std::vector<float> uvs(Positions.size(), 0.0f);

        m_VertexArray = std::make_unique<Core::VertexArray>();
        m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(positions, 2));
        m_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(uvs, 2));

        // 2. 建立索引與 VertexArray
    std::vector<unsigned int> indices;
        for (int i = 0; i < static_cast<int>(positions.size()/2); ++i) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }
        std::vector<unsigned int> indices1 = { 0, 1, 2, 2, 3, 0 };
        m_IndexBuffer = std::make_unique<Core::IndexBuffer>(indices);
        m_VertexArray->SetIndexBuffer(std::move(m_IndexBuffer));

        // 3. 綁定框架內建的 Shader [1]
        m_Program = std::make_unique<Core::Program>(
        "../PTSD/assets/shaders/Base.vert",
            "../PTSD/assets/shaders/Base.frag"
        );

        // 4. 綁定 UniformBuffer 接收矩陣資料
        // 注意：名稱 ("Transform" 或 "Matrices") 需與 Base.vert 內的 uniform block 對應
        m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
            *m_Program, "Transform", 0
        );

        // 5. 生成我們的 1x1 像素純色紋理
        ApplyColorToTexture();
}

void CustomColorShape::ApplyColorToTexture() {
    // 取出 rgba 值 (Util::Color 繼承自 glm::vec4 [6])
    Uint8 data[] = {
        static_cast<Uint8>(m_Color.r),
        static_cast<Uint8>(m_Color.g),
        static_cast<Uint8>(m_Color.b),
        static_cast<Uint8>(m_Color.a)
    };

    if (!m_Texture) {
        // GL_RGBA 格式建立紋理 [8, 9]
        m_Texture = std::make_unique<Core::Texture>(GL_RGBA, 1, 1, data);
    } else {
        // 如果紋理已存在，直接利用 UpdateData 覆寫這 1 個像素的顏色 (效能極高) [10]
        m_Texture->UpdateData(GL_RGBA, 1, 1, data);
    }
}

void CustomColorShape::SetColors(Util::Color color) {
    // 將顏色匯入
    m_Color = color;
    ApplyColorToTexture(); // 更新那 1 個像素的透明度
}

void CustomColorShape::Draw(const Core::Matrices &data){
    m_Program->Bind();                  // 啟動 Shader [11]
    m_UniformBuffer->SetData(0, data);  // 將 GameObject 的矩陣變換送入 Shader [5]
    m_Texture->Bind(0);                 // 綁定我們的 1x1 純色紋理 [10]

    m_VertexArray->Bind();              // 綁定形狀 [12]
    m_VertexArray->DrawTriangles();     // 畫出多邊形 [13]
}

glm::vec2 CustomColorShape::GetSize() const{
    return {1.0f, 1.0f};
}