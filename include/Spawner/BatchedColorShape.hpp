//
// Created by jerry on 2026/4/9.
//

#ifndef JUST_SHAPES_AND_BEATS_BATCHEDCOLORSHAPE_HPP
#define JUST_SHAPES_AND_BEATS_BATCHEDCOLORSHAPE_HPP

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

class BatchedColorShape : public Core::Drawable {
private:
    std::unique_ptr<Core::VertexArray> m_VertexArray = nullptr; // 初始為空
    std::unique_ptr<Core::Program> m_Program;
    std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_UniformBuffer;
    std::unique_ptr<Core::Texture> m_Texture;

    // 累積頂點用的陣列
    std::vector<float> m_Positions;
    std::vector<float> m_UVs;
    std::vector<unsigned int> m_Indices;

    Util::Color m_Color;
public:
    BatchedColorShape(const Util::Color& color);

    void BeginBatch();

    void AddQuad(const std::vector<float>& worldVertices, const std::vector<float>& worldUVs);

    void EndBatch();

    void Draw(const Core::Matrices &data) override;

    glm::vec2 GetSize() const override { return {1.0f, 1.0f}; }
};

#endif //JUST_SHAPES_AND_BEATS_BATCHEDCOLORSHAPE_HPP