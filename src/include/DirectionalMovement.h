#ifndef DIRECTIONALMOVEMENT_H
#define DIRECTIONALMOVEMENT_H

#include <SFML/System/Vector2.hpp>
#include "MovementStrategy.h"
#include <cmath>
#include <memory>


class DirectionalMovement : public MovementStrategy {
public:
    DirectionalMovement(float direction, float velocity, float acceleration,
                       float maxVelocity = 99.0f, float minVelocity = 1.0f);

    void update(float deltaTime) override;
    sf::Vector2f getMovementVector() const override;
    
    // Setters
    void setDirection(float degrees) override;
    void setVelocity(float velocity);
    void setAcceleration(float acceleration);
    void setMaxVelocity(float maxVelocity);
    void setMinVelocity(float minVelocity);
    
    // Getters
    float getDirection() const;
    float getVelocity() const;
    float getAcceleration() const;
    float getMaxVelocity() const;
    float getMinVelocity() const;

    virtual std::shared_ptr<MovementStrategy> clone() const override;

private:
    float direction;    // en grados (0-360)
    float velocity;     // magnitud de la velocidad
    float acceleration; // magnitud de la aceleración
    float maxVelocity;
    float minVelocity;

    float clampVelocity(float vel) const;
    float normalizeAngle(float degrees) const;
};

#endif // DIRECTIONALMOVEMENT_H