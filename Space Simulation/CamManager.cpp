#include "CamManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


CamManager::CamManager(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : camPos(position), camUp(up), yaw(yaw), pitch(pitch),
    speed(10.0f), sensitivity(0.1f)
{
    camFront = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCamVectors();
}

// Atualiza a câmera com base no teclado
void CamManager::processKeyboard(float deltaTime)
{
    float velocity = speed * deltaTime;
    if (Input::isKeyPressed(GLFW_KEY_W))
        camPos += camFront * velocity; // frente
    if (Input::isKeyPressed(GLFW_KEY_S))
        camPos -= camFront * velocity; // trás
    if (Input::isKeyPressed(GLFW_KEY_A))
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * velocity; // esquerda
    if (Input::isKeyPressed(GLFW_KEY_D))
        camPos += glm::normalize(glm::cross(camFront, camUp)) * velocity; // direita
    if (Input::isKeyPressed(GLFW_KEY_SPACE))
        camPos += camUp * velocity; // sobe
    if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        camPos -= camUp * velocity; // desce
}

// Atualiza a câmera com base no movimento do mouse
void CamManager::processMouse()
{
    if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        float xoffset = static_cast<float>(Input::getDeltaX()) * sensitivity;
        float yoffset = static_cast<float>(Input::getDeltaY()) * sensitivity;

        yaw += xoffset;
        pitch -= yoffset;

        // Limita pitch para evitar flip
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        updateCamVectors();
    }
}

// Retorna a matriz view
glm::mat4 CamManager::getViewMatrix() const
{
    return glm::lookAt(camPos, camPos + camFront, camUp);
}

// Atualiza o vetor front baseado no yaw e pitch
void CamManager::updateCamVectors()
{
    camFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront.y = sin(glm::radians(pitch));
    camFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(camFront);
}
