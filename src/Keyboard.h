#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Keyboard {
public:
    static bool keys[1024];

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static bool key(int key);
};

class Mouse {
public:
    static double x, y;
    static double lastX, lastY;
    static double dx, dy;
    static double scrollX, scrollY;
    static bool firstMouse;
    static bool buttons[GLFW_MOUSE_BUTTON_LAST];

    // Callbacks de GLFW
    static void cursorPosCallback(GLFWwindow* window, double _x, double _y);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void WheelCallback(GLFWwindow* window, double _dx, double _dy);

    // Funciones de utilidad
    static double getDX();
    static double getDY();
    static double getScrollY();
};