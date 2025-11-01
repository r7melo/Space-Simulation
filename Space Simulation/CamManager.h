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

        // Variáveis internas
        float yaw;
        float pitch;
        float speed;
        float sensitivity;

    public:
        // Construtor com valores padrão
        CamManager(glm::vec3 position = glm::vec3(0.0f, 0.0f, 50.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f,
            float pitch = 0.0f);

        // Atualiza a câmera com base no teclado
        void processKeyboard(float deltaTime);

        // Atualiza a câmera com base no movimento do mouse
        void processMouse();

        // Retorna a matriz view da câmera
        glm::mat4 getViewMatrix() const;

        // Posição e orientação
        glm::vec3 camPos;
        glm::vec3 camFront;
        glm::vec3 camUp;
};


#endif