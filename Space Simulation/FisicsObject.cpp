#include "FisicsObject.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

// Construtor
FisicsObject::FisicsObject(double m, double r, glm::dvec3 p, glm::dvec3 v)
    : massa(m), raio(r), pos(p), vel(v) {
}

// Getters
glm::dvec3 FisicsObject::getPos() const { return pos; }
double FisicsObject::getRaio() const { return raio; }

// Atualiza posição e velocidade
bool FisicsObject::atualizar(const FisicsObject& outro, double dt, double& distancia) {
    glm::dvec3 r_vec = pos - outro.pos;
    double r_mag = glm::length(r_vec);

    if (r_mag == 0.0) {
        distancia = 0.0;
        return true; // colisão ou mesma posição
    }

    glm::dvec3 r_hat = r_vec / r_mag;

    // Força gravitacional
    glm::dvec3 F_grav = G * massa * outro.massa / (r_mag * r_mag) * r_hat;

    // Atualiza velocidade e posição do objeto
    glm::dvec3 a = F_grav / massa; // aceleração sobre este objeto
    vel -= a * dt;                 // "vel -= " porque a força puxa para outro
    pos += vel * dt;

    // Distância e colisão
    distancia = glm::length(pos - outro.pos);
    return distancia <= (raio + outro.raio);
}
