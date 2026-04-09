#include "../Camera.h"

Camera::Camera(glm::vec3 position)
    : cameraPos(position),
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    yaw(-90.0f),
    pitch(0.0f),
    movementSpeed(2.5f),
    mouseSensitivity(0.1f),
    zoom(45.0f),
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
    isFrozen(false),
    mode(CameraMode::FIRST_PERSON),
    targetPos(position)
{
    updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy) {
    if (isFrozen) return;

    yaw += dx * mouseSensitivity;
    pitch += dy * mouseSensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}
void Camera::updateCameraPos(CameraDirection dir, double deltaTime) {
    if (isFrozen) return;

    float velocity = movementSpeed * deltaTime;

    if (dir == CameraDirection::FORWARD)
        cameraPos += cameraFront * velocity;
    if (dir == CameraDirection::BACKWARD)
        cameraPos -= cameraFront * velocity;
    if (dir == CameraDirection::LEFT)
        cameraPos -= cameraRight * velocity;
    if (dir == CameraDirection::RIGHT)
        cameraPos += cameraRight * velocity;
    if (dir == CameraDirection::UP)
        cameraPos += cameraUp * velocity;
    if (dir == CameraDirection::DOWN)
        cameraPos -= cameraUp * velocity;
}
void Camera::updateCameraZoom(double dy) {
    if (zoom >= 1.0f && zoom <= 45.0f){
        zoom -= dy * mouseSensitivity;
    } else if (zoom < 1.0f) {
        zoom = 1.0f;
    } else {
        zoom = 45.0f;
    }
}

glm::mat4 Camera::GetViewMatrix() {
    if (mode == CameraMode::THIRD_PERSON) {
        // Camera looks at target from behind
        glm::vec3 offset = glm::vec3(0.0f, 2.0f, 5.0f); // Adjust as needed
        cameraPos = targetPos - cameraFront * offset.z + glm::vec3(0.0f, offset.y, 0.0f);
        return glm::lookAt(cameraPos, targetPos, cameraUp);
    } else if (mode == CameraMode::SPECTATOR) {
        // Free camera
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    } else {
        // First person
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::Freze() {
    isFrozen = true;
}

void Camera::Unfreeze() {
    isFrozen = false;
}

void Camera::SetMode(CameraMode newMode) {
    mode = newMode;
}

void Camera::SetTarget(const glm::vec3& target) {
    targetPos = target;
}