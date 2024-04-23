#pragma once
#include "GL/glew.h"
#include <vector>
#include "cassert"

namespace Renderer {

struct VertexBufferLayoutElement {
    unsigned int type;
    int count;
    bool normalized;

    static int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferLayoutElement> m_elements;
    int m_stride;
public:
    VertexBufferLayout();

    template<typename T>
    void Push(int count);

    template<>
    void Push<float>(int count);

    template<>
    void Push<unsigned int>(int count);

    template<>
    void Push<unsigned char>(int count);

    inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_elements; }
    inline int GetStride() const { return m_stride; }
};

} // Renderer