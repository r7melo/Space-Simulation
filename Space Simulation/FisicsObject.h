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

    // Atualiza a posi��o e velocidade em rela��o a outro objeto
    // Retorna true se houver colis�o
    bool atualizar(const FisicsObject& outro, double dt, double& distancia);

    // Adiciona uma for�a externa ao objeto (acumula)
    void aplicarForca(const glm::dvec3& forca);

    // Atualiza posi��o e velocidade usando todas as for�as acumuladas
    void integrar(double dt);

private:
    double massa;
    double raio;
    glm::dvec3 pos;
    glm::dvec3 vel;
    glm::dvec3 forcaAcumulada = glm::dvec3(0.0); // soma de todas as for�as

    static constexpr double G = 1.0;
};

#endif
