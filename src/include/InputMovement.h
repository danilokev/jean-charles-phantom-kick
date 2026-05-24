#ifndef INPUTMOVEMENT_H
#define INPUTMOVEMENT_H

#include <SFML/System/Vector2.hpp>
#include "MovementStrategy.h"
#include <memory>

class InputMovement : public MovementStrategy {
public:
    InputMovement(float speed = 3.5f);

    void update(float deltaTime) override;
    void setDirection(float degrees) override;
    sf::Vector2f getMovementVector() const override;
    std::shared_ptr<MovementStrategy> clone() const override;

    void setSpeed(float speed);
    float getSpeed() const;

private:
    float speed;
    sf::Vector2f movementVector;

    sf::Vector2f computeInputDirection() const;
};

#endif // INPUTMOVEMENT_H
