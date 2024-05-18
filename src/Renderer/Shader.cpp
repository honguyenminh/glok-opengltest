#include "Shader.h"
#include "GL/glew.h"
#include <sstream>
#include <iostream>
#include <cassert>
#include <fstream>

namespace Renderer {

Shader::Shader(const std::string& filepath) // NOLINT(*-pass-by-value)
    : m_filePath(filepath), m_rendererId(0) {
    TryParseShader();
    CreateProgram();
}

Shader::~Shader() {
    for (auto i : m_createdShaderId) {
        glDeleteShader(i);
    }
    glDeleteShader(m_rendererId);
}

void Shader::Bind() const {
    glUseProgram(m_rendererId);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string &name, int value) {
    Bind();
    int location = GetUniformLocation(name);
    if (location == -1)
        std::cout << "Warning! shader uniform " << name << " doesn't exist\n";
    glUniform1i(location, value);
}

void Shader::SetUniform4f(const std::string &name, float v1, float v2, float v3, float v4) {
    Bind();
    int location = GetUniformLocation(name);
    if (location == -1)
        std::cout << "Warning! shader uniform " << name << " doesn't exist\n";
    glUniform4f(location, v1, v2, v3, v4);
}

int Shader::GetUniformLocation(const std::string &name) {
    // cache the looked up uniform
    if (m_uniformLocationCache.contains(name))
        return m_uniformLocationCache.at(name);

    int location = glGetUniformLocation(m_rendererId, name.c_str());
    m_uniformLocationCache[name] = location;
    return location;
}

bool Shader::TryParseShader() {
    std::ifstream stream(m_filePath);

    unsigned int type = 0;

    std::string line;
    std::stringstream ss[2];
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = GL_VERTEX_SHADER;
            else if (line.find("fragment") != std::string::npos)
                type = GL_FRAGMENT_SHADER;
        } else {
            assert(type != 0);
            ss[type == GL_FRAGMENT_SHADER] << line << '\n';
        }
    }

    m_source = { ss[0].str(), ss[1].str() };
    return true;
}

unsigned int Shader::Compile(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "ERROR: Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        delete[] message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

void Shader::CreateProgram() {
    assert(m_rendererId == 0);
    unsigned int program = glCreateProgram();
    unsigned int vs = Compile(GL_VERTEX_SHADER, m_source.VertexSource);
    unsigned int fs = Compile(GL_FRAGMENT_SHADER, m_source.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    m_rendererId = program;
}

} // Renderer