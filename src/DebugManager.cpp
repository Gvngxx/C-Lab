#include "DebugManager.h"
#include <iostream>

DebugManager::DebugManager(GLFWwindow* window, ShaderProgram* shader)
    : window(window), shader(shader)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // --- AQUÍ AGRANDAMOS EL TEXTO ---
    io.FontGlobalScale = 1.4f; 

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();
}

DebugManager::~DebugManager() {
    // 1. Apaga el renderizador de OpenGL3
    ImGui_ImplOpenGL3_Shutdown();
    
    // 2. Apaga la conexión con GLFW
    ImGui_ImplGlfw_Shutdown();
    
    // 3. Destruye el contexto (la memoria que usa ImGui)
    ImGui::DestroyContext();
}

void DebugManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugManager::render() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(260, 260), ImVec2(1920, 1080));
    ImGui::SetNextWindowSize(ImVec2(260, 260), ImGuiCond_Appearing);
    ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Appearing);
    ImGui::SetNextWindowCollapsed(true, ImGuiCond_Appearing);

    char buf[128];
    sprintf(buf, "Debug Menu %c %d###AnimatedTitle", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) % 3]);

    ImGui::Begin(buf, NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
    
    // Obtenemos el ancho disponible para que los elementos sepan cuánto espacio tienen
    float contentWidth = ImGui::GetContentRegionAvail().x;

    // -- AQUÍ MOSTRAMOS INFORMACIÓN DEl RENDIMIENTO --
    if (ImGui::CollapsingHeader("Rendimiento", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextWrapped("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::TextWrapped("CPU Usage: %.1f%%", ImGui::GetIO().Framerate > 0 ? (100.0f / ImGui::GetIO().Framerate) : 0.0f);
        ImGui::TextWrapped("Frame Rate: %.3f", 1000.0f / ImGui::GetIO().Framerate);
    }
    ImGui::Separator();

    // -- AQUÍ MOSTRAMOS INFORMACIÓN DE VENTANA E INPUT --
    if (ImGui::CollapsingHeader("Window & Input", ImGuiTreeNodeFlags_None)) {
        ImGui::TextWrapped("Window Size: %.0f x %.0f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

        ImGui::TextWrapped("Mouse on Menu: %s", ImGui::GetIO().WantCaptureMouse ? "Yes" : "No");
        ImGui::TextWrapped("Mouse Position: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);

        ImGui::Text("Keys down:");
        ImGui::SameLine();
        for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key++) {
            if (ImGui::IsKeyDown((ImGuiKey)key)) {
                ImGui::SameLine();
                ImGui::Text("[%s]", ImGui::GetKeyName((ImGuiKey)key) ? ImGui::GetKeyName((ImGuiKey)key) : "Uknown");
            }
        }
    }
    ImGui::Separator();

    // -- AQUÍ MOSTRAMOS INFORMACIÓN DEl SHADER --
    if (ImGui::CollapsingHeader("Shaders", ImGuiTreeNodeFlags_None)) {
        if (ImGui::Checkbox("Modo Wireframe", &wireframeMode)) {
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        }
    }

    // -- AQUÍ MOSTRAMOS LA DEMO Y EXTRAS --
    if (ImGui::CollapsingHeader("Extra", ImGuiTreeNodeFlags_None)) {
        if (ImGui::Checkbox("Show Demo", &showDemoWindow)) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        } else {
            if (showDemoWindow) {
                ImGui::ShowDemoWindow(&showDemoWindow);
            }
        }
    }
    ImGui::End();
}

void DebugManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}