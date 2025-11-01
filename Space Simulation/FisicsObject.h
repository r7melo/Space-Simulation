#ifndef FISICSOBJECT_H
#define FISICSOBJECT_H

#include <glm/glm.hpp>

class FisicsObject {
public:
    // Construtor
    FisicsObject(double massa, double raio, glm::dvec3 pos, glm::dvec3 vel);

    // Getters
    glm::dvec3 getPos() const;
    double getRaio() const;

    // Atualiza a posição e velocidade em relação a outro objeto
    // Retorna true se houver colisão
    bool atualizar(const FisicsObject& outro, double dt, double& distancia);

private:
    double massa;
    double raio;
    glm::dvec3 pos;
    glm::dvec3 vel;

    static constexpr double G = 1.0;
};

#endif
