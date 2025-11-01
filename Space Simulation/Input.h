#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <unordered_map>

class Input {
public:
    // Inicializa com a janela GLFW
    static void init(GLFWwindow* window);

    // Atualizar deltas do mouse (chame a cada frame)
    static void update();

    // Consultar teclas
    static bool isKeyPressed(int key);
    static bool isMouseButtonPressed(int button);

    // Dados do mouse
    static double getMouseX();
    static double getMouseY();
    static double getDeltaX();
    static double getDeltaY();

private:
    static GLFWwindow* s_window;

    static std::unordered_map<int, bool> s_keys;
    static std::unordered_map<int, bool> s_mouseButtons;

    static double s_mouseX, s_mouseY;
    static double s_lastMouseX, s_lastMouseY;
    static double s_deltaX, s_deltaY;

    // Callbacks GLFW
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif
