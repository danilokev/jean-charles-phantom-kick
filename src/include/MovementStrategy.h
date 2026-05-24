#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <SFML/System/Vector2.hpp>
#include <memory>

class MovementStrategy {
public:
    virtual ~MovementStrategy() noexcept = default;

    virtual void update(float deltaTime) = 0;
    virtual sf::Vector2f getMovementVector() const = 0;
    virtual void setDirection(float degrees) = 0;
    virtual std::shared_ptr<MovementStrategy> clone() const = 0;

};

#endif // MOVEMENTSTRATEGY_H
