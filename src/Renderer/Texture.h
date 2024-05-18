#pragma once
#include "Renderer.h"

namespace Renderer {

class Texture {
private:
    unsigned int m_rendererId;
    std::string m_filePath;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_bitsPerPixel;
public:
    Texture(std::string  path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
};

}