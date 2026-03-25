#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"
#include <iostream>

// Callback para atrapar errores de GLFW (ayuda mucho al debuggear)
void error_callback(int error, const char* description) {
    std::cerr << "Error GLFW: " << description << std::endl;
}

// Callback para que el dibujo no se deforme al estirar la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW";
        return -1;
    }

    // Configuración de versión 3.3 Core (moderna)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Fan | V0.1.0", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar punteros de OpenGL (imprescindible para que funcione glDrawArrays y demás)
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Error al inicializar GLAD" << std::endl;
        return -1;
    }

    // Datos del Cuadrado (2 triángulos que comparten puntos)
    float vertices[] = {
        // Primer triángulo
        -0.5f, -0.5f, 0.0f, 
        -0.5f,  0.5f, 0.0f, 
         0.5f,  0.5f, 0.0f, 

        // Segundo triángulo
         0.5f,  0.5f, 0.0f, 
         0.5f, -0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f  
    };

    // VAO (Contenedor de configuración) y VBO (Caja de datos en la GPU)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Definimos que cada vértice tiene 3 floats (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // Código fuente de los Shaders (se pasan a tu clase ShaderProgram)
    const char* vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() { gl_Position = vec4(aPos, 1.0); }\n";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(0.1, 0.8, 0.2, 1.0); }\n";

    // Instancia de tu clase que compila y activa los shaders
    ShaderProgram shader(vertexShaderSource, fragmentShaderSource);

    // --- Bucle principal ---
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Color de fondo
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usamos el programa de shaders
        shader.use();
        
        // Dibujamos el objeto
        glBindVertexArray(VAO);
        // IMPORTANTE: Cambiado a 6 porque el cuadrado tiene 6 puntos en total
        glDrawArrays(GL_TRIANGLES, 0, 6); 
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // Limpieza de memoria
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}