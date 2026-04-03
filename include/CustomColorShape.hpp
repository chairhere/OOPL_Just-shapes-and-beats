//
// Created by jerry on 2026/3/26.
//

#ifndef JUST_SHAPES_AND_BEATS_CUSTOMCOLORSHAPE_HPP
#define JUST_SHAPES_AND_BEATS_CUSTOMCOLORSHAPE_HPP

#include "Core/Drawable.hpp"
#include "Core/VertexArray.hpp"
#include "Core/VertexBuffer.hpp"
#include "Core/IndexBuffer.hpp"
#include "Core/Program.hpp"
#include "Core/UniformBuffer.hpp"
#include "Core/Texture.hpp"
#include "Util/Color.hpp"
#include "config.hpp" // 為了使用 PTSD_ASSETS_DIR 巨集
#include <vector>
#include <memory>

class CustomColorShape : public Core::Drawable {
private:
    std::unique_ptr<Core::VertexArray> m_VertexArray;
    std::unique_ptr<Core::VertexBuffer> m_VertexBuffer;
    std::unique_ptr<Core::IndexBuffer> m_IndexBuffer;

    // 補齊上次缺少的 Shader、UniformBuffer 與 Texture
    std::unique_ptr<Core::Program> m_Program;
    std::unique_ptr<Core::UniformBuffer<Core::Matrices>> m_UniformBuffer;
    std::unique_ptr<Core::Texture> m_Texture;

    Util::Color m_Color;

public:
    explicit CustomColorShape(const Util::Color& color, const std::vector<float> &Positions);

    // 將當前的 Util::Color 轉為 1x1 紋理寫入 GPU
    void ApplyColorToTexture();

    // 專門為淡入淡出提供的動態透明度介面

    void SetColors(Util::Color color);

    // 實作 Drawable 的 Draw，這一次把所有管線都串起來了！
    void Draw(const Core::Matrices &data) override;

    glm::vec2 GetSize() const override;

};

#endif //JUST_SHAPES_AND_BEATS_CUSTOMCOLORSHAPE_HPP