#pragma once

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class CameraMode {
    FIRST_PERSON,
    THIRD_PERSON,
    SPECTATOR
};

class Camera {
public:
    glm::vec3 cameraPos;

    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    glm::vec3 worldUp;

    float pitch;
    float yaw;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    bool isFrozen;
    CameraMode mode;
    glm::vec3 targetPos; // For third person, position to look at

    Camera(glm::vec3 position);

    void updateCameraDirection(double dx, double dy);
    void updateCameraPos(CameraDirection dir, double deltaTime);
    void updateCameraZoom(double dy);

    void SetMode(CameraMode newMode);
    void SetTarget(const glm::vec3& target);

    void Freze();
    void Unfreeze();

    glm::mat4 GetViewMatrix();

private:
    void updateCameraVectors();
};