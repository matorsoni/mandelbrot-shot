#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include<string>
GLuint create_shader_program(const std::string& vert_file,
                             const std::string& frag_file);

int main()
{
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // #TODO: make it resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Mandelbrot Zoom", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to create OpenGL context.\n";
        glfwTerminate();
        return -1;
    }

    glClearColor(115.f/255, 38.f/255, 115.f/255, 1.f);
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << "\n";

    // Setup quad.
    const float quad[12] = {
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
    };
    const unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), (void*)quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), (void*)indices, GL_STATIC_DRAW);

    // Shader program.
    GLuint shader_program = create_shader_program("../src/vert.glsl",
                                                  "../src/frag.glsl");
    glUseProgram(shader_program);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


#include <fstream> // for std::ifstream
//#include <utility> // for std::move
static std::string load_text_file(const std::string& filename)
{
    std::string text, line;
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Unable to read file " << filename << "\n";
        return {};
    }

    while (getline(file, line)) {
        text.append(line + "\n");
    }

    //std::move(text);
    return text;
}

GLuint create_shader_program(const std::string& vert_file, const std::string& frag_file)
{
    // Load vertex shader source and compile it.
    std::string vert_shader = load_text_file(vert_file);
    const char* vert_shader_src = vert_shader.c_str();
    GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader_id, 1, &vert_shader_src, NULL);
    glCompileShader(vert_shader_id);

    // Load fragment shader source and compile it.
    std::string frag_shader = load_text_file(frag_file);
    const char* frag_shader_src = frag_shader.c_str();
    unsigned int frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader_id, 1, &frag_shader_src, NULL);
    glCompileShader(frag_shader_id);

    // Check if shaders compiled correctly.
    {
        int success;
        char info_log[512];

        glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vert_shader_id, 512, NULL, info_log);
            std::cout << "Vertex shader compilation failed: \n" << info_log << "\n";
        }

        glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag_shader_id, 512, NULL, info_log);
            std::cout << "Fragment shader compilation failed: \n" << info_log << "\n";
        }
    }

    // Link shader program.
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_shader_id);
    glAttachShader(program, frag_shader_id);
    glLinkProgram(program);

    // Check if linking went well.
    {
        int success;
        char info_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, info_log);
            std::cout << "Shader program linking failed: \n" << info_log << "\n";
        }
    }

    // Delete intermediate shader objects.
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);

    return program;
}
