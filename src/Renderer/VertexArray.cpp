#include "VertexArray.h"

namespace Renderer {

VertexArray::VertexArray() {
    // generate new vertex array to hold vertex buffer and vertex attributes layout of that buffer
    glGenVertexArrays(1, &m_rendererId);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout &layout) {
    Bind(); // Select current array
    vb.Bind(); // "select" the buffer
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        // generate new vertex attributes layout of above buffer
        glEnableVertexAttribArray(i); // id start from 0
        // this will bind attribute with the buffer for some reason
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
                              reinterpret_cast<const void *>(offset));
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(m_rendererId);
}
void VertexArray::Unbind() {
    glBindVertexArray(0);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_rendererId);
}

} // Renderer