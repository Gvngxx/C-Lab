#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shaprog.h"
#include "triangle.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>

std::string readFile(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    glfwInit();

    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "NORAD | By LucidVertx", NULL, NULL);
    if (window == NULL) {
        std::cerr << "[-] Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    } else {
        std::cout << "[+] Successfully created Window" << std::endl;
        std::cout << "[+] FILE: " << std::filesystem::current_path() << std::endl;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "[-] Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    std::string vertexCode = readFile("assets/shaders/vertx.glsl");
    std::string fragmentCode = readFile("assets/shaders/frag.glsl");
    ShaderProgram shader(vertexCode.c_str(), fragmentCode.c_str());
    
    Triangle triangle;

    int MoveA = glGetAttribLocation(shader.id(), "Time");

    // Loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate shader
        shader.bind();

        shader.setFloat("Time", (std::sin(glfwGetTime()) + 1.0f) / 3); // Move between 0.0 and 1.0 over time

        // input
        processInput(window);

        // render
        triangle.Render(shader.id());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    triangle.CleanUp();
    shader.unbind();
    shader.CleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_TRUE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
    } else if(glfwGetKey(window, GLFW_KEY_F5) == GLFW_TRUE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // ReFill
    }

    /* if(glfwGetKey(window, GLFW_KEY_D) == GLFW_TRUE) {
        int MoveA = glGetAttribLocation(shaderProgramID, "a");

        glUniform1f(MoveA, 0.5f);
    }*/
}