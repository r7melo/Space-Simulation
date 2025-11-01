#ifndef FISICSOBJECT_H
#define FISICSOBJECT_H

#include <glm/glm.hpp>

class FisicsObject {
public:
    // Construtor
    FisicsObject(double massa, double raio, glm::dvec3 pos, glm::dvec3 vel);

    // Getters
    glm::dvec3 getPos() const;
    glm::dvec3 getVel() const;
    double getRaio() const;

    // Atualiza a posição e velocidade em relação a outro objeto
    // Retorna true se houver colisão
    bool atualizar(const FisicsObject& outro, double dt, double& distancia);

    // Adiciona uma força externa ao objeto (acumula)
    void aplicarForca(const glm::dvec3& forca);

    // Atualiza posição e velocidade usando todas as forças acumuladas
    void integrar(double dt);

private:
    double massa;
    double raio;
    glm::dvec3 pos;
    glm::dvec3 vel;
    glm::dvec3 forcaAcumulada = glm::dvec3(0.0); // soma de todas as forças

    static constexpr double G = 1.0;
};

#endif
