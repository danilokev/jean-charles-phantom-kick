#include "../include/TurningMovement.h"
#include <cmath>

namespace { constexpr float PI = 3.14159265358979323846f; }

TurningMovement::TurningMovement(float initialDirDeg, float vel,
                                 float trigTime, float degToTurn, float turnSpd)
    : direction(normalize(initialDirDeg)),
      speed(vel),
      triggerTime(trigTime),
      degreesToTurn(degToTurn),
      turnSpeed(turnSpd) {}

void TurningMovement::update(float dt)
{
    // 1) Acumular tiempo hasta que toque empezar el giro
    elapsed += dt;
    if (!turning && elapsed >= triggerTime)
        turning = true;

    // 2) Si estamos girando, avanzar el giro
    if (turning && std::abs(turnedSoFar) < std::abs(degreesToTurn)) {
        float delta = turnSpeed * dt;
        if (degreesToTurn < 0) delta = -delta;

        direction    = normalize(direction + delta);
        turnedSoFar += delta;

        // Evitar pasarse del ángulo objetivo
        if ((degreesToTurn >= 0 && turnedSoFar > degreesToTurn) ||
            (degreesToTurn <  0 && turnedSoFar < degreesToTurn))
        {
            float overshoot = turnedSoFar - degreesToTurn;
            direction       = normalize(direction - overshoot);
            turnedSoFar     = degreesToTurn;
        }
    }
}

sf::Vector2f TurningMovement::getMovementVector() const
{
    float rad = direction * PI / 180.f;
    return { speed * std::cos(rad), speed * std::sin(rad) };
}

void TurningMovement::setDirection(float deg) { direction = normalize(deg); }

std::shared_ptr<MovementStrategy> TurningMovement::clone() const
{
    return std::make_shared<TurningMovement>(*this);   // copia completa
}

float TurningMovement::normalize(float deg)
{
    deg = std::fmod(deg, 360.f);
    if (deg < 0) deg += 360.f;
    return deg;
}
