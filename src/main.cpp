#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

struct ShaderProgramSource {
    std::string VertexSource, FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath) {
    std::ifstream stream(filepath);

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

    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "ERROR: Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static void errorHandler(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar *message, const void *userParam) {
    char const *_source;
    char const *_type;
    char const *_severity;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            _source = "UNKNOWN";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UNDEFINED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            _type = "OTHER";
            break;
        case GL_DEBUG_TYPE_MARKER:
            _type = "MARKER";
            break;
        default:
            _type = "UNKNOWN";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            _severity = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            _severity = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            _severity = "LOW";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            _severity = "NOTIFICATION";
            break;
        default:
            _severity = "UNKNOWN";
            break;
    }

    printf("[%s] %s severity, id %d, raised from %s: %s\n",
           _type, _severity, id, _source, message);
}

int main() {
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    // Set opengl version to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Set opengl profile to core profile, you need to set opengl version for this to work
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // uncomment this and put monitor into glfwCreateWindow to set it to full screen
//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//    if (!monitor) {
//        glfwTerminate();
//        return -1;
//    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Enable vsync essentially
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew!\n";
    }

    std::cout << glGetString(GL_VERSION) << '\n';

    glEnable(GL_DEBUG_OUTPUT);
#ifndef NDEBUG
    // to allow breakpoints with callstack in the handler
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
    glDebugMessageCallback(errorHandler, nullptr); // passes the debug handler and no user data (second arg)

    // vertex positions buffer
    #define VERTEX_CNT 4
    #define VERTEX_DIM 2
    #define POS_SIZE VERTEX_CNT * VERTEX_DIM
    float positions[POS_SIZE] = {
            -0.5f, -0.5f,
            0.5f, 0.5f,
            0.5f, -0.5f,
            -0.5f, 0.5f
    };
    // index buffer
    // holds which position from vertex pos buffer is in a shape
    #define INDEX_CNT 6
    unsigned int indices[] = {
            0, 1, 2, // shape 1
            0, 1, 3  // shape 2,... etc
    };

    // generate new vertex array to hold vertex buffer and vertex attributes layout of that buffer
    unsigned int vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // generate new vertex buffer
    Renderer::VertexBuffer vertexBuffer(positions, POS_SIZE * sizeof(float));

    // generate new vertex attributes layout of above buffer
    glEnableVertexAttribArray(0); // id start from 0
    // this will bind attribute with the buffer for some reason
    glVertexAttribPointer(0, VERTEX_DIM, GL_FLOAT, GL_FALSE, VERTEX_DIM * sizeof(float), nullptr);

    // generate new index buffer from above indices
    Renderer::IndexBuffer indexBuffer = {indices, INDEX_CNT};

    // compile the shader to interpret the vertex data and actually draw things
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    // set initial color in the uniform
    int location = glGetUniformLocation(shader, "u_Color");
    assert(location != -1);
    glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    // reset/unbind everything
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float r = 0.0f; // red value, to animate
    float increment = 0.005f; // increment step

    // until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // select compiled shader
        glUseProgram(shader);
        // set color in the uniform
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        // bind/select the above vertex array, this will auto select vertex data and attribute layout for us
        glBindVertexArray(vertexArrayObj);
        // bind the index buffer
        indexBuffer.Bind();

        // draw call
        glDrawElements(GL_TRIANGLES, INDEX_CNT, GL_UNSIGNED_INT, 0);

        // animating the red value
        if (r > 1.0f)
            increment = -0.005f;
        else if (r < 0.0f)
            increment = 0.005f;

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // clean up shaders
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}