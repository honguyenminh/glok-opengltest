#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

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
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        glfwTerminate();
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(680, 480, "Hello World", nullptr, nullptr);
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
    #define VERTEX_DIM 4
    #define POS_SIZE VERTEX_CNT * VERTEX_DIM
    float positions[POS_SIZE] = {
            -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, 0.5f,    1.0f, 1.0f,
            0.5f, -0.5f,   1.0f, 0.0f,
            -0.5f, 0.5f,   0.0f, 1.0f
    };
    // index buffer
    // holds which position from vertex pos buffer is in a shape
    #define INDEX_CNT 6
    unsigned int indices[] = {
            0, 1, 2, // shape 1
            0, 1, 3  // shape 2,... etc
    };

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // generate new vertex array to hold vertex buffer and vertex attributes layout of that buffer
    Renderer::VertexArray vertexArray;

    // generate new vertex buffer
    Renderer::VertexBuffer vertexBuffer(positions, POS_SIZE * sizeof(float));
    Renderer::VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    vertexArray.AddBuffer(vertexBuffer, layout);

    // generate new index buffer from above indices
    Renderer::IndexBuffer indexBuffer = {indices, INDEX_CNT};

    // compile the shader to interpret the vertex data and actually draw things
    Renderer::Shader shader("res/shaders/Basic.shader");

    Renderer::Texture texture("res/textures/sync-flat.png");
    const int slot = 0;
    texture.Bind(slot);
    shader.SetUniform1i("u_Texture", slot);

    // reset/unbind everything
    shader.Unbind();
    vertexArray.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();

    Renderer::Renderer renderer;

    float r = 0.0f; // red value, to animate
    float increment = 0.005f; // increment step

    // until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();
        // select compiled shader
        shader.Bind();

        renderer.Draw(vertexArray, indexBuffer, shader);

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


    //! We might need to clean up shaders here

    glfwTerminate();
    return 0;
}