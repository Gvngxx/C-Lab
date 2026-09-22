#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "../ShaderProgram.h"
#include "../Models.h"
#include "../Camera.h"
#include "../Keyboard.h"
#include "../DebugManager.h"
#include "../entities/Player.h"
#include "../VersionManager.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
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

    glEnable(GL_DEPTH_TEST);
    // glEnable();

    glfwSetKeyCallback(window, Keyboard::KeyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);
    
    // Use normal cursor by default so the camera works over VNC and on touchpads.
    // Press F2 to enable raw mouse capture if you want local relative motion.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    std::string vertexCode = readFile("assets/shaders/vertx.glsl");
    std::string fragmentCode = readFile("assets/shaders/frag.glsl");
    ShaderProgram shader(vertexCode.c_str(), fragmentCode.c_str());

    Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
    camera.SetMode(CameraMode::SPECTATOR); // SPECTATOR,THIRD_PERSON
    
    // Piedra + Textura
    DMalla Datos01 = Reader::Import("assets/models/test.fbx");
    Cube cube("assets/textures/Texture.png");
    Render testObj;
    testObj.Conf(Datos01);

    // Pizo (No tocar)
    Floor floor("assets/textures/Texture.png");

    // Jugador +
    Skin skin("assets/textures/red.png");
    Player player("assets/textures/red.png");

    // --- MATRIZ DE PROYECCIÓN ---
    // Define el campo de visión (45 grados), el ratio de aspecto y qué tan cerca/lejos vemos.
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)1280 / (float)720, 0.1f, 100.0f);

    // --- MATRIZ DE VISTA ---
    // Obtiene la posición y dirección actual de la cámara
    glm::mat4 view = camera.GetViewMatrix();
    DebugManager debug{window, &shader};

    float deltatime = 0.0f;
    float lastFrame = 0.0f;

    // Loop
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
        shader.setMat4("projection", &projection[0][0]);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", &view[0][0]);

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", &model[0][0]);

        // ---- Input ----
        processInput(window, deltatime, camera);
        double mouseDX = Mouse::getDX();
        double mouseDY = Mouse::getDY();
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED || Mouse::buttons[GLFW_MOUSE_BUTTON_LEFT]) {
            camera.updateCameraDirection(mouseDX, mouseDY);
        }
        camera.updateCameraZoom(Mouse::getScrollY());

        // Update the player and keep the camera following it.
        player.Update(deltatime);
        player.SetCameraTarget(camera);

        // --- Render ---
        debug.beginFrame();
        debug.render();

        // Pos del cube y shaders
        cube.Render(shader.id(), model,
            0.4f, -0.5f, -3.7f,      // Pos
            1.0f, 1.0f, 1.0f,       // Size
            0.0f);                 // rotation
        testObj.Draw(); // Target para dibujar el objeto de arriba

        skin.Render(shader.id(), model,
            1.0f, 1.0f, 1.0f,       // Size
            0.0f);                 // rotation

        // Render del pizo (No tocar)
        floor.Render(shader.id(), model,
            0.0f, -0.51f, 0.0f,        // Pos
            5.0f, 1.0f, 5.0f,        // Size
            90.0f);                 // rotation

        // Render del player
        player.Render(shader.id(), model);

        debug.endFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.CleanUp();
    skin.CleanUp();
    floor.CleanUp();
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
    if(Keyboard::keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, true);
    }

    if(Keyboard::keys[GLFW_KEY_H]) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        camera.Freze();
        Mouse::firstMouse = true;
    }
    if(Keyboard::keys[GLFW_KEY_J]) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera.Unfreeze();
        Mouse::firstMouse = true;
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

    // Keyboard camera rotation fallback for VNC / trackpad input
    if(Keyboard::keys[GLFW_KEY_LEFT])
        camera.updateCameraDirection(-100.0 * deltaTime, 0.0);
    if(Keyboard::keys[GLFW_KEY_RIGHT])
        camera.updateCameraDirection(100.0 * deltaTime, 0.0);
    if(Keyboard::keys[GLFW_KEY_UP])
        camera.updateCameraDirection(0.0, 100.0 * deltaTime);
    if(Keyboard::keys[GLFW_KEY_DOWN])
        camera.updateCameraDirection(0.0, -100.0 * deltaTime);
    if(Keyboard::keys[GLFW_KEY_PAGE_UP])
        camera.updateCameraZoom(-1.0 * deltaTime * 50.0);
    if(Keyboard::keys[GLFW_KEY_PAGE_DOWN])
        camera.updateCameraZoom(1.0 * deltaTime * 50.0);
}