#include "../Player.h"
#include "../../Models.h"
#include "../../Camera.h"
#include <iostream>

Player::Player(const std::string& texturePath) : cube(nullptr), position(0.0f, 0.0f, 0.0f), velocity(0.0f), onGround(true), gravity(-9.8f), jumpForce(5.0f), groundY(0.0f) {
    cube = new Cube(texturePath);
}

Player::~Player() {
    if (cube) {
        cube->CleanUp();
        delete cube;
    }
}

void Player::Update(float deltaTime) {
    // Apply gravity
    if (!onGround) {
        velocity.y += gravity * deltaTime;
    }

    // Update position
    position += velocity * deltaTime;

    // Check ground collision
    if (position.y <= groundY) {
        position.y = groundY;
        velocity.y = 0.0f;
        onGround = true;
    } else {
        onGround = false;
    }

    // Simple friction
    velocity.x *= 0.9f;
    velocity.z *= 0.9f;
}

void Player::Render(unsigned int shaderProgramID, const glm::mat4& modelMatrix) {
    if (cube) {
        cube->Render(shaderProgramID, modelMatrix, position.x, position.y, position.z, 1.0f, 1.0f, 1.0f, 0.0f);
    }
}

void Player::Jump() {
    if (onGround) {
        velocity.y = jumpForce;
        onGround = false;
    }
}

void Player::Move(float dx, float dy, float dz) {
    velocity.x += dx;
    velocity.y += dy;
    velocity.z += dz;
}

void Player::SetCameraTarget(Camera& camera) {
    camera.SetTarget(position);
}