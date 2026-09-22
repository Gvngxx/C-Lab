#include "../../Models.h"
#include "../../TextureLoader.h"
#include <iostream>

Cube::Cube(const std::string& texturePath) : textureID(0) {
    // Load texture
    textureID = TextureLoader::LoadTexture(texturePath);

    // === VÉRTICES DEL CUBO ===
    // Cada vértice tiene (X, Y, Z, U, V)
    // El cubo va de -0.5 a +0.5 en cada eje
    /*
    float vertx[] = {
        // Cara frontal (Z = +0.5)
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // Vértice 0
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // Vértice 1
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // Vértice 2
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // Vértice 3

        // Cara trasera (Z = -0.5)
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // Vértice 4
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // Vértice 5
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // Vértice 6
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f   // Vértice 7
    }; */

    float vertx[] = {
        // Cara frontal (Z = +0.5)
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // Vértice 0
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // Vértice 1
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // Vértice 2
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // Vértice 3

        // Cara trasera (Z = -0.5)
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // Vértice 4
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // Vértice 5
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // Vértice 6
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f   // Vértice 7
    };

    // === ÍNDICES DEL CUBO ===
    // Cada cara se divide en 2 triángulos → 6 caras × 2 = 12 triángulos × 3 vértices = 36 índices
    unsigned int indices[] = {
        // Cara frontal (Z = +0.5) — vértices 0,1,2,3
        0, 1, 2,  // Triángulo 1
        2, 3, 0,  // Triángulo 2

        // Cara trasera (Z = -0.5) — vértices 4,5,6,7
        5, 4, 7,  // Triángulo 1 (orden inverso para que no se vea al revés)
        7, 6, 5,  // Triángulo 2

        // Cara superior (Y = +0.5) — vértices 3,2,6,7
        3, 2, 6,  // Triángulo 1
        6, 7, 3,  // Triángulo 2

        // Cara inferior (Y = -0.5) — vértices 0,1,5,4
        0, 4, 5,  // Triángulo 1
        5, 1, 0,  // Triángulo 2

        // Cara derecha (X = +0.5) — vértices 1,5,6,2
        1, 2, 6,  // Triángulo 1
        6, 5, 1,  // Triángulo 2

        // Cara izquierda (X = -0.5) — vértices 0,4,7,3
        0, 3, 7,  // Triángulo 1
        7, 4, 0   // Triángulo 2
    };

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // Se guardan las configs que yo ago

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertx), vertx, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); // Se deja de guardar configs
}

void Cube::Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation) {
    
    glm::mat4 NGenModel = modelMatrix;
    NGenModel = glm::translate(NGenModel, glm::vec3(x, y, z));
    NGenModel = glm::scale(NGenModel, glm::vec3(SizeX, SizeY, SizeZ));
    NGenModel = glm::rotate(NGenModel, glm::radians(rotation), glm::vec3(0.0f,  1.0f,  0.0f));
    
    // Usar el programa shader
    glUseProgram(shaderProgramID);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgramID, "texture1"), 0);

    // Activar el VAO
    glBindVertexArray(VAO);

    // === ENVIAR LA MATRIZ MODEL ===
    // Esta es la clave para mover el cubo
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(NGenModel));

    // === DIBUJAR EL CUBO ===
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Desactivar el VAO
    glBindVertexArray(0);
}

void Cube::CleanUp() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (textureID != 0) {
        TextureLoader::UnloadTexture(textureID);
    }
}
