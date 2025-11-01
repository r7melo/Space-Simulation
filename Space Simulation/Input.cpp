#include "Input.h"
#include <iostream>

// Inicializações estáticas
GLFWwindow* Input::s_window = nullptr;
std::unordered_map<int, bool> Input::s_keys;
std::unordered_map<int, bool> Input::s_mouseButtons;
double Input::s_mouseX = 0.0, Input::s_mouseY = 0.0;
double Input::s_lastMouseX = 0.0, Input::s_lastMouseY = 0.0;
double Input::s_deltaX = 0.0, Input::s_deltaY = 0.0;

// ------------------------------------------------------
// Implementação dos métodos públicos
// ------------------------------------------------------

void Input::init(GLFWwindow* window) {
    s_window = window;
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

void Input::update() {
    s_deltaX = s_mouseX - s_lastMouseX;
    s_deltaY = s_mouseY - s_lastMouseY;
    s_lastMouseX = s_mouseX;
    s_lastMouseY = s_mouseY;
}

bool Input::isKeyPressed(int key) {
    return s_keys[key];
}

bool Input::isMouseButtonPressed(int button) {
    return s_mouseButtons[button];
}

double Input::getMouseX() { return s_mouseX; }
double Input::getMouseY() { return s_mouseY; }
double Input::getDeltaX() { return s_deltaX; }
double Input::getDeltaY() { return s_deltaY; }

// ------------------------------------------------------
// Callbacks GLFW
// ------------------------------------------------------

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        s_keys[key] = true;
    else if (action == GLFW_RELEASE)
        s_keys[key] = false;

    std::cout << "Tecla: " << key << " | Pressed: " << s_keys[key] << std::endl;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS)
        s_mouseButtons[button] = true;
    else if (action == GLFW_RELEASE)
        s_mouseButtons[button] = false;

    std::cout << "Mouse button: " << button << " | Pressed: " << s_mouseButtons[button] << std::endl;
}

void Input::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    s_mouseX = xpos;
    s_mouseY = ypos;
    std::cout << "Mouse pos: (" << xpos << ", " << ypos << ")" << std::endl;
}
