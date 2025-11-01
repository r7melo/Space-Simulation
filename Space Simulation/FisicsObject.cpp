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
glm::dvec3 FisicsObject::getVel() const { return glm::dvec3(); }
double FisicsObject::getRaio() const { return raio; }

// Atualiza posição e velocidade em relação a outro objeto
bool FisicsObject::atualizar(const FisicsObject& outro, double dt, double& distancia) {
    glm::dvec3 r_vec = pos - outro.pos;
    double r_mag = glm::length(r_vec);

    if (r_mag == 0.0) {
        distancia = 0.0;
        return true;
    }

    glm::dvec3 r_hat = r_vec / r_mag;

    // Força gravitacional
    glm::dvec3 F_grav = G * massa * outro.massa / (r_mag * r_mag) * r_hat;

    aplicarForca(-F_grav); // força sobre este objeto
    integrar(dt);           // atualiza posição e velocidade

    distancia = glm::length(pos - outro.pos);
    return distancia <= (raio + outro.raio);
}

// Adiciona força externa (pode chamar várias vezes antes de integrar)
void FisicsObject::aplicarForca(const glm::dvec3& forca) {
    forcaAcumulada += forca;
}

// Integra movimento usando todas as forças acumuladas
void FisicsObject::integrar(double dt) {
    glm::dvec3 aceleracao = forcaAcumulada / massa;
    vel += aceleracao * dt;
    pos += vel * dt;

    // Zera forças acumuladas para o próximo passo
    forcaAcumulada = glm::dvec3(0.0);
}
