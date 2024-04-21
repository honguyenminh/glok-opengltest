#pragma once

namespace Renderer {

class VertexBuffer {
private:
    unsigned int m_rendererId;
public:
    /// Generate a vertex buffer
    /// \param data The data to be put into created vertex buffer
    /// \param size size of \p data in bytes
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    static void Unbind();
};

} // Renderer
