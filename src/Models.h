#pragma once

#include <string>
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertice {
    float posicion[3];
    float coordsTextura[2];
};

struct DMalla {
    std::vector<Vertice> vertices;
    std::vector<unsigned int> indices;
};

class Reader {
public:
    // Lee un archivo y te devuelve los datos de la malla listos
    static DMalla Import(const std::string& ruta);
};

class Render {
public:
    Render();
    ~Render();

    // Sube los datos crudos a la GPU
    void Conf(const DMalla& malla);
    
    // Pinta los datos en la pantalla
    void Draw();

private:
    unsigned int VAO, VBO, EBO;
    int numIndices;
};

class Cube {
public:
    Cube(const std::string& texturePath);
    void Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation);
    void CleanUp();
private:
    unsigned int VBO, VAO, EBO, textureID;
};

class Floor {
public:
    Floor(const std::string& texturePath);
    void Render(unsigned int shaderProgramID, const glm::mat4& baseModel, float x, float y, float z, float SizeX, float SizeY, float SizeZ, float rotation);
    void CleanUp();
    bool CheckAABBCollision(const glm::vec3& point, float halfSize) const;
private:
    unsigned int VBO, VAO, EBO, textureID;
};