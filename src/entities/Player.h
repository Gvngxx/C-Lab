#pragma once

#include <string>
#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera;
class Skin {
public:
    Skin(const std::string& texturePath);
    glm::vec3 GetPosition();
    void Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix, const glm::vec3& position, float SizeX, float SizeY, float SizeZ, float rotation);
    void CleanUp();
private:
    glm::vec3 position;
    unsigned int VBO, VAO, EBO, textureID;
};

class Player {
public:
    Player(const std::string& texturePath);
    ~Player();

    void Update(float deltaTime);
    void Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix);
    void Jump();
    void Move(float dx, float dy, float dz);
    void SetCameraTarget(Camera& camera);

private:
    Skin* skin;
    glm::vec3 position;
    glm::vec3 velocity;
    bool onGround;
    float gravity;
    float jumpForce;
    float groundY;
};