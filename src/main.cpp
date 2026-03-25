#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"
#include <iostream>

// Función de callback para errores de GLFW
void error_callback(int error, const char* description) {
    std::cerr << "Error GLFW: " << description << std::endl;
}

// Función de callback para redimensionar la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Configurar callback de error
    glfwSetErrorCallback(error_callback);

    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW";
        return -1;
    }

    // Configurar GLFW para OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Fan | V0.1.0", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Hacer el contexto de la ventana actual
    glfwMakeContextCurrent(window);

    // Configurar callback de redimensionamiento
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar punteros de función de OpenGL con GLAD
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    // Crear datos para un triangulo
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.1, 0.8, 0.2, 1.0);\n"
        "}\n";

    ShaderProgram shader(vertexShaderSource, fragmentShaderSource);

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Procesar eventos
        glfwPollEvents();

        // Limpiar pantalla
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }

    // Limpiar y terminar
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}