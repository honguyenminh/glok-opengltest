#include "VertexBufferLayout.h"

int Renderer::VertexBufferLayoutElement::GetSizeOfType(unsigned int type) {
    switch (type) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        default: assert(false); return 0;
    }
}

template<>
void Renderer::VertexBufferLayout::Push<float>(int count) {
    m_elements.push_back({ GL_FLOAT, count, false });
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void Renderer::VertexBufferLayout::Push<unsigned char>(int count) {
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, false });
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

template<>
void Renderer::VertexBufferLayout::Push<unsigned int>(int count) {
    m_elements.push_back({ GL_UNSIGNED_INT, count, false });
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<typename T>
void Renderer::VertexBufferLayout::Push(int count) {
//    static_assert(false);
}

Renderer::VertexBufferLayout::VertexBufferLayout() : m_stride(0) {}
