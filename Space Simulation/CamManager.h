#ifndef CAM_MANAGER_H
#define CAM_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

// ===================================================
// Classe Camera
// ===================================================
class CamManager {
    private:
        // Atualiza o vetor front baseado no yaw e pitch
        void updateCamVectors();

        // Vari�veis internas
        float yaw;
        float pitch;
        float speed;
        float sensitivity;

    public:
        // Construtor com valores padr�o
        CamManager(glm::vec3 position = glm::vec3(0.0f, 0.0f, 50.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f,
            float pitch = 0.0f);

        // Atualiza a c�mera com base no teclado
        void processKeyboard(float deltaTime);

        // Atualiza a c�mera com base no movimento do mouse
        void processMouse();

        // Retorna a matriz view da c�mera
        glm::mat4 getViewMatrix() const;

        // Posi��o e orienta��o
        glm::vec3 camPos;
        glm::vec3 camFront;
        glm::vec3 camUp;
};


#endif