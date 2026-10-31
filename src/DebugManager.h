#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <string>

namespace microverse {
class Simulation;
}

class DebugManager {
public:
    DebugManager(GLFWwindow* window, ShaderProgram* shader);
    ~DebugManager();

    void beginFrame();
    void render();
    void endFrame();
    void setSimulation(microverse::Simulation* simulation);

private:
    GLFWwindow* window;
    ShaderProgram* shader;
    microverse::Simulation* simulation = nullptr;

    // UI state
    bool showDemoWindow = false;
    bool showAnotherWindow = false;
    bool wireframeMode = false;

    float sliderValue = 0.5f;
    int counter = 0;
    float customTime = 0.0f;
};