#pragma once
#include "GL/glew.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Renderer {
class Renderer {
public:
    void Clear() const;
    void Draw(const VertexArray&, const IndexBuffer&, const Shader&) const;
};
}
