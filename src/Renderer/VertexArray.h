#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Renderer {

class VertexArray {
private:
    unsigned int m_rendererId;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

    void Bind() const;
    static void Unbind() ;
};

} // Renderer
