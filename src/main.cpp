#include <glad/gl.h>
#include <GLFW/glfw3.h>
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
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    // Configurar GLFW para OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ejemplo de Ventana OpenGL", NULL, NULL);
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

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Procesar eventos
        glfwPollEvents();

        // Limpiar pantalla con color azul
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Intercambiar buffers
        glfwSwapBuffers(window);
    }

    // Limpiar y terminar
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}