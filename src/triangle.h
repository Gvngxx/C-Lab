#pragma once

#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Triangle {
public:
    Triangle();
    void Render(unsigned int shaderProgramID);

    void CleanUp();

private:
    unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object
};