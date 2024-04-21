#pragma once

namespace Renderer {

class IndexBuffer {
private:
    unsigned int m_rendererId;
    unsigned int m_count;
public:
    /// Generate an index buffer
    /// \param data Array of indices to put inside the buffer
    /// \param count Count of indices in \p data
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    static void Unbind() ;

    unsigned int GetCount() const;
};

} // Renderer
