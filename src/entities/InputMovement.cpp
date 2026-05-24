#include "../include/InputMovement.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

InputMovement::InputMovement(float speed)
    : speed(speed), movementVector(0.f, 0.f) {}

void InputMovement::update(float deltaTime) {
    sf::Vector2f inputDir = computeInputDirection();
    
    if (inputDir.x != 0.f || inputDir.y != 0.f) {
        float magnitude = std::sqrt(inputDir.x * inputDir.x + inputDir.y * inputDir.y);
        sf::Vector2f normalizedDir = inputDir / magnitude;
        movementVector = normalizedDir * speed * deltaTime;
    } else {
        movementVector = sf::Vector2f(0.f, 0.f);
    }
}

sf::Vector2f InputMovement::getMovementVector() const {
    return movementVector;
}

void InputMovement::setSpeed(float speed) {
    this->speed = speed;
}

float InputMovement::getSpeed() const {
    return speed;
}

sf::Vector2f InputMovement::computeInputDirection() const {
    sf::Vector2f direction(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        direction.y -= 2.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        direction.y += 2.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        direction.x -= 2.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        direction.x += 2.f;
    return direction;
}

void InputMovement::setDirection(float /*degrees*/) {}

std::shared_ptr<MovementStrategy> InputMovement::clone() const {
    return std::make_shared<InputMovement>(speed);
}
