#include "IndexBuffer.h"

#include "GL/glew.h"
#include <cassert>

namespace Renderer {

IndexBuffer::IndexBuffer(const unsigned int* data, int count)
    : m_count(count) {
    assert(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_rendererId);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int IndexBuffer::GetCount() const {
    return m_count;
}

} // Renderer