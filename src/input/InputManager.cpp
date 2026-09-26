#include "../Keyboard.h"
#include <iostream>

// Inicializamos las variables estáticas
bool Keyboard::keys[1024] = { false };

double Mouse::x = 0;
double Mouse::y = 0;
double Mouse::lastX = 0;
double Mouse::lastY = 0;
double Mouse::dx = 0;
double Mouse::dy = 0;
double Mouse::scrollX = 0;
double Mouse::scrollY = 0;
bool Mouse::firstMouse = true;
bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { false };
int Mouse::ignoredMotionFrames = 0;

// --- KEYBOARD ---
void Keyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

bool Keyboard::key(int key) {
    return keys[key];
}

// --- MOUSE ---
void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
    x = _x;
    y = _y;

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = x - lastX;
    dy = lastY - y; // Invertido porque en pantalla Y crece hacia abajo

    lastX = x;
    lastY = y;
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) buttons[button] = true;
        else if (action == GLFW_RELEASE) buttons[button] = false;
    }
}

void Mouse::WheelCallback(GLFWwindow* window, double _dx, double _dy) {
    scrollX = _dx;
    scrollY = _dy;
}

double Mouse::getDX() {
    if (ignoredMotionFrames > 0) {
        return 0;
    }

    double _dx = dx;
    dx = 0; // Limpiamos el delta para que no se quede girando solo
    return _dx;
}

double Mouse::getDY() {
    if (ignoredMotionFrames > 0) {
        --ignoredMotionFrames;
        dx = 0;
        dy = 0;
        return 0;
    }

    double _dy = dy;
    dy = 0;
    return _dy;
}

double Mouse::getScrollY() {
    double _scrollY = scrollY;
    scrollY = 0;
    return _scrollY;
}

void Mouse::resetMotion() {
    dx = 0;
    dy = 0;
    lastX = x;
    lastY = y;
    firstMouse = true;
    ignoredMotionFrames = 2;
}