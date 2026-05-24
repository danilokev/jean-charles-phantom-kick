#include "../include/DirectionalMovement.h"
#include <cmath>

const float PI = 3.14159265358979323846f;

DirectionalMovement::DirectionalMovement(float direction, float velocity, float acceleration,
                                       float maxVelocity, float minVelocity)
    : direction(normalizeAngle(direction)), velocity(velocity), acceleration(acceleration),
      maxVelocity(maxVelocity), minVelocity(minVelocity) {}

void DirectionalMovement::update(float deltaTime) {
    if (acceleration != 0.0f) {
        float newVelocity = velocity + acceleration * deltaTime;
        
        // Solo actualizamos la velocidad si está dentro de los límites o si la aceleración
        // la está llevando hacia dentro de los límites
        if ((velocity < maxVelocity && velocity > minVelocity) || 
            (acceleration > 0 && velocity < maxVelocity) ||
            (acceleration < 0 && velocity > minVelocity)) {
            velocity = clampVelocity(newVelocity);
        }
    }
}

sf::Vector2f DirectionalMovement::getMovementVector() const {
    float radians = direction * PI / 180.0f;
    return sf::Vector2f(
        velocity * std::cos(radians),
        velocity * std::sin(radians)
    );
}

// Setters
void DirectionalMovement::setDirection(float degrees) {
    direction = normalizeAngle(degrees);
}

void DirectionalMovement::setVelocity(float vel) {
    velocity = clampVelocity(vel);
}

void DirectionalMovement::setAcceleration(float acc) {
    acceleration = acc;
}

void DirectionalMovement::setMaxVelocity(float maxVel) {
    maxVelocity = maxVel;
    velocity = clampVelocity(velocity);
}

void DirectionalMovement::setMinVelocity(float minVel) {
    minVelocity = minVel;
    velocity = clampVelocity(velocity);
}

// Getters
float DirectionalMovement::getDirection() const { return direction; }
float DirectionalMovement::getVelocity() const { return velocity; }
float DirectionalMovement::getAcceleration() const { return acceleration; }
float DirectionalMovement::getMaxVelocity() const { return maxVelocity; }
float DirectionalMovement::getMinVelocity() const { return minVelocity; }

// Helpers
float DirectionalMovement::clampVelocity(float vel) const {
    if (vel > maxVelocity) return maxVelocity;
    if (vel < minVelocity) return minVelocity;
    return vel;
}

float DirectionalMovement::normalizeAngle(float degrees) const {
    degrees = std::fmod(degrees, 360.0f);
    if (degrees < 0) degrees += 360.0f;
    return degrees;
}

std::shared_ptr<MovementStrategy> DirectionalMovement::clone() const {
    return std::make_shared<DirectionalMovement>(direction, velocity, acceleration, maxVelocity, minVelocity);
}