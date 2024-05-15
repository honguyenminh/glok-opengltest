#include "Renderer.h"

namespace Renderer {

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const {
    // select compiled shader
    shader.Bind();
    // bind/select the above vertex array, this will auto select vertex data and attribute layout for us
    vertexArray.Bind();
    // bind the index buffer
    indexBuffer.Bind();

    // draw call
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}
}