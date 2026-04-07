#pragma once

#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
public:
    Cube();
    void Render(unsigned int shaderProgramID, glm::vec3 position, glm::vec3 size, glm::vec3 rotation = glm::vec3(1.0f, 0.0f, 0.0f));

    void CleanUp();

private:
    unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object
};

class Floor {
public:
    Floor();
    void Render(unsigned int shaderProgramID);

    void CleanUp();

private:
    unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object
};