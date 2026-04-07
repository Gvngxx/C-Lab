#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Models.h"
#include "Camera.h"
#include "Keyboard.h"
#include "DebugManager.h"
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
void processInput(GLFWwindow* window, double deltaTime, Camera& camera);

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

    glfwSetKeyCallback(window, Keyboard::KeyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
    
    // En VNC el cursor debe estar normal para que los eventos se reciban correctamente.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    std::string vertexCode = readFile("assets/shaders/vertx.glsl");
    std::string fragmentCode = readFile("assets/shaders/frag.glsl");
    ShaderProgram shader(vertexCode.c_str(), fragmentCode.c_str());

    Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

    // --- MATRIZ DE PROYECCIÓN ---
    // Define el campo de visión (45 grados), el ratio de aspecto y qué tan cerca/lejos vemos.
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)1280 / (float)720, 0.1f, 100.0f);

    // --- MATRIZ DE VISTA ---
    // Obtiene la posición y dirección actual de la cámara
    glm::mat4 view = camera.GetViewMatrix();\
    DebugManager debug{window, &shader};

    Cube cube;

    float deltatime = 0.0f;
    float lastFrame = 0.0f;
    Mouse::scrollY = 0;

    // Loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Limpiar pantalla
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();

        // 2. ENVIAR MATRICES DENTRO DEL LOOP
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
        shader.setMat4("projection", &projection[0][0]); 

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", &view[0][0]);

        glm::mat4 model = camera.GetViewMatrix();
        shader.setMat4("model", &model[0][0]);

        // ---- Input ----
        processInput(window, deltatime, camera);
        camera.updateCameraDirection(Mouse::getDX(), Mouse::getDY());

        // --- Render ---
        debug.beginFrame();
        debug.render();

        cube.Render(shader.id(), glm::vec3(0.0f, -1.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(glfwGetTime() * 50.0f, 0.0f, 0.0f));

        debug.endFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.CleanUp();
    shader.unbind();
    shader.CleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, double deltaTime, Camera& camera) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_TRUE) {
        camera.Freeze();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_TRUE) {
        camera.Unfreeze();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if(Keyboard::keys[GLFW_KEY_W])
        camera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
    if(Keyboard::keys[GLFW_KEY_D])
        camera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
    if(Keyboard::keys[GLFW_KEY_A])
        camera.updateCameraPos(CameraDirection::LEFT, deltaTime);
    if(Keyboard::keys[GLFW_KEY_S])
        camera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
    if(Keyboard::keys[GLFW_KEY_Q]) 
        camera.updateCameraPos(CameraDirection::UP, deltaTime);
    if(Keyboard::keys[GLFW_KEY_Z])
        camera.updateCameraPos(CameraDirection::DOWN, deltaTime);
}