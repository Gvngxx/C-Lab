#include "../../../Models.h"
#include "../../../TextureLoader.h"
#include <iostream>

Table::Table(const std::string& texturePath) : textureID(0) {
    // Load texture
    textureID = TextureLoader::LoadTexture(texturePath);
    float vertx[] = {
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };

    // === INDICES DEL CUBO ===
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        5, 4, 7,
        7, 6, 5,
        3, 2, 6,
        6, 7, 3,
        0, 4, 5,
        5, 1, 0,
        1, 2, 6,
        6, 5, 1,
        0, 3, 7,
        7, 4, 0
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertx), vertx, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); // Save
}

void Table::Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation) {
    
    glm::mat4 NGenModel = modelMatrix;
    NGenModel = glm::translate(NGenModel, glm::vec3(x, y, z));
    NGenModel = glm::scale(NGenModel, glm::vec3(SizeX, SizeY, SizeZ));
    NGenModel = glm::rotate(NGenModel, glm::radians(rotation), glm::vec3(0.0f,  1.0f,  0.0f));
    NGenModel = glm::rotate(NGenModel, glm::radians(rotation), glm::vec3(1.0f,  0.0f,  0.0f));
    
    // Usar el programa shader
    glUseProgram(shaderProgramID);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 0);

    // Activar el VAO
    glBindVertexArray(VAO);

    // === ENVIAR LA MATRIZ MODEL ===
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(NGenModel));

    return;

    // === DIBUJAR EL CUBO ===
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Desactivar el VAO
    glBindVertexArray(0);
}

void Table::CleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (textureID != 0) {
        TextureLoader::UnloadTexture(textureID);
    }
}
