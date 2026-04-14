//
// Created by jerry on 2026/4/14.
//

#ifndef JUST_SHAPES_AND_BEATS_BATCHEDCIRCLESHAPE_HPP
#define JUST_SHAPES_AND_BEATS_BATCHEDCIRCLESHAPE_HPP

#include "Core/Drawable.hpp"
#include "Core/VertexArray.hpp"
#include "Core/VertexBuffer.hpp"
#include "Core/IndexBuffer.hpp"
#include "Core/Program.hpp"
#include "Core/UniformBuffer.hpp"
#include "Core/Texture.hpp"
#include "Util/Color.hpp"
#include "config.hpp"
#include <vector>
#include <memory>

class BatchedCircleShape : public Core::Drawable {
private:
    std::unique_ptr<Core::VertexArray> m_VertexArray = nullptr; // 初始為空
    std::unique_ptr<Core::Program> m_Program;
    std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_UniformBuffer;
    std::unique_ptr<Core::Texture> m_Texture;

    // 累積頂點用的陣列
    std::vector<float> m_Positions;
    std::vector<float> m_UVs;
    std::vector<unsigned int> m_Indices;

    Util::Color m_Color{};
    // ... VBO, VAO 等與您之前設計的 Quad 相同

public:
    BatchedCircleShape(const Util::Color& color);

    void BeginBatch();

    void AddQuad(const std::vector<float>& worldVertices, const std::vector<float>& worldUVs, const std::vector<float>& localVertices);

    void EndBatch();

    void Draw(const Core::Matrices &data) override;

    glm::vec2 GetSize() const override { return {1.0f, 1.0f}; }
};
#endif //JUST_SHAPES_AND_BEATS_BATCHEDCIRCLESHAPE_HPP