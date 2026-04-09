#include "../../Models.h"
#include <iostream>

Floor::Floor() {
    // === VÉRTICES DEL CUBO ===
    // Cada vértice tiene (X, Y, Z)
    float vertx[] = {
        -1.0f, -1.0f,  0.0f,  // Vértice 0
        1.0f, -1.0f,  0.0f,  // Vértice 1
        1.0f,  1.0f,  0.0f,  // Vértice 2
        -1.0f,  1.0f,  0.0f   // Vértice 3
    };

    // === ÍNDICES DEL CUBO ===
    // Cada cara se divide en 2 triángulos
    unsigned int indices[] = {
        // Cara — vértices 0,1,2,3
        0, 1, 2,  // Triángulo 1
        0, 2, 3  // Triángulo 2
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // Se guardan las configs que yo ago

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertx), vertx, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // Se deja de guardar configs
}

void Floor::Render(unsigned int shaderProgramID, const glm::mat4& baseModel, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation) {
    
    glm::mat4 NGenModel = baseModel;
    NGenModel = glm::translate(NGenModel, glm::vec3(x, y, z));
    NGenModel = glm::scale(NGenModel, glm::vec3(SizeX, SizeY, SizeZ));
    NGenModel = glm::rotate(NGenModel, glm::radians(rotation), glm::vec3(1.0f,  0.0f,  0.0f));

    glUseProgram(shaderProgramID);
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(NGenModel));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Floor::CleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Floor::CheckAABBCollision(const glm::vec3& point, float halfSize) const {
    // Simple AABB collision check
    // Floor is at y=0, assume it's a plane, but for AABB, check against the floor's bounding box
    // For simplicity, assume floor is from -1 to 1 in x and z, at y=0
    return (point.x >= -1.0f && point.x <= 1.0f &&
            point.z >= -1.0f && point.z <= 1.0f &&
            point.y - halfSize <= 0.0f); // Touching or below floor
}
