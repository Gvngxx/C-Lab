#include "Models.h"
#include <iostream>

Floor::Floor() {
    // Vertx
    float vertx[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    // Indice de los Vertx
    unsigned int indices[] = {
        0, 1, 3, // Triangulo 1
        1, 2, 3 // Triangulo 2
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

void Floor::Render(unsigned int shaderProgramID) {
    glUseProgram(shaderProgramID); // Conecta el shader al programa

    glBindVertexArray(VAO); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Dibuja los Arrays
    glBindVertexArray(0);
}

void Floor::CleanUp() {
    glDeleteVertexArrays(1, &VAO); // Se borra el VAO
    glDeleteBuffers(1, &VBO); // Se borra el VBO
}