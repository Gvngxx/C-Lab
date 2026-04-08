#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    Cube();
    void Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation);
    void CleanUp();

private:
    unsigned int VBO, VAO, EBO;
};

class Floor {
public:
    Floor();
    void Render(unsigned int shaderProgramID, const glm::mat4& baseModel, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation);
    void CleanUp();

private:
    unsigned int VBO, VAO, EBO;
};