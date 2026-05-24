#ifndef TURNINGMOVEMENT_H
#define TURNINGMOVEMENT_H

#include "MovementStrategy.h"
#include <SFML/System/Vector2.hpp>
#include <memory>

/**
 * Estrategia “gira‑por‑tiempo”.
 * - Se mueve recto con velocidad constante.
 * - Cuando transcurre `triggerTime` segundos, empieza a girar
 *   `degreesToTurn` grados a razón de `turnSpeed` grados/segundo.
 * - Al completar el giro, continúa en la nueva dirección sin más lógica.
 */
class TurningMovement : public MovementStrategy {
public:
    TurningMovement(float initialDirDeg,
                    float velocity,
                    float triggerTime,      // s desde el spawn hasta iniciar el giro
                    float degreesToTurn,    // + o –  (p.e. 360 para un loop)
                    float turnSpeedDeg);    // grados/seg

    void update(float dt) override;
    sf::Vector2f getMovementVector() const override;
    void setDirection(float deg) override;
    std::shared_ptr<MovementStrategy> clone() const override;

private:
    float direction;        // grados
    float speed;            // unidades/seg

    // control temporal del giro
    float triggerTime;      // s
    float elapsed = 0.f;    // s desde spawn

    // datos del giro
    float degreesToTurn;
    float turnSpeed;        // grados/s
    bool  turning   = false;
    float turnedSoFar = 0.f;

    static float normalize(float deg);
};

#endif /* TURNINGMOVEMENT_H */
