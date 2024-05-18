#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Renderer {

struct ShaderProgramSource {
    std::string VertexSource, FragmentSource;
};

class Shader {
private:
    std::string m_filePath;
    ShaderProgramSource m_source;
    std::vector<unsigned int> m_createdShaderId;
    std::unordered_map<std::string, int> m_uniformLocationCache;
    unsigned int m_rendererId;
    // cache for uniforms
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    static void Unbind();

    // Set uniforms
    void SetUniform1i(const std::string& name, int);
    void SetUniform4f(const std::string& name, float, float, float, float);
private:
    bool TryParseShader();
    int GetUniformLocation(const std::string& name);
    void CreateProgram();
    /// Compile a shader of given type from given source
    /// \param type Type of shader to compile, for example `GL_VERTEX_SHADER`
    /// \param source Source of the shader
    /// \return Id of the compiled shader
    static unsigned int Compile(unsigned int type, const std::string &source);
};

} // Renderer
