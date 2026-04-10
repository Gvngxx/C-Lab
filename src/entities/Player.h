#pragma once

#include <string>
#include <glm/glm.hpp>

class Cube;
class Camera;

class Player {
public:
    Player(const std::string& texturePath);
    ~Player();

    void Update(float deltaTime);
    void Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix);
    void Jump();
    void Move(float dx, float dy, float dz);

    glm::vec3 GetPosition() const;
    void SetCameraTarget(Camera& camera);

private:
    Cube* cube;
    glm::vec3 position;
    glm::vec3 velocity;
    bool onGround;
    float gravity;
    float jumpForce;
    float groundY;
};