#include "../include/PlayerShoot.h"
#include "../include/DirectionalMovement.h"
#include <cmath>

PlayerShoot::PlayerShoot(const sf::Vector2f &startPos, const int shootType, const sf::Vector2f &direction, float speed, float damageValue)
    : position(startPos),
      collision(startPos, 5.f),
      damage(damageValue),
      direction(direction)
{
    Sprite sprite;
    switch (shootType) {
        case 0:
        this->sprite = Sprite(DISPARO, BLUE);
            break;
        case 1:
        this->sprite = Sprite(GOTA, WHITE);
            break;
        case 2:
        this->sprite = Sprite(DISPARO, YELLOW);
            break;
        case 3:
        this->sprite = Sprite(ORBE_ANIMADO, BLUE);
            break;
        default:
        this->sprite = Sprite(ESFERA, WHITE); // Color por defecto
            break;
    }
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f normDir = (length != 0.f) ? direction / length : sf::Vector2f(0.f, -1.f);
    movementStrategy = std::make_shared<DirectionalMovement>(0.f, speed, 0.f, speed, speed);
    float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159265f;
    movementStrategy->setDirection(angle);
    sprite.setPosition(position.x, position.y);
    sprite.setDirection(direction);
}

void PlayerShoot::update(float deltaTime) {
    if (movementStrategy) {
        movementStrategy->update(deltaTime);
        position += movementStrategy->getMovementVector() * deltaTime;
    }
    sprite.setPosition(position.x, position.y);
    collision.setCenter(position);
    // sprite.updateAnimation(deltaTime);
}

void PlayerShoot::render(sf::RenderWindow &window) const {
    window.draw(sprite.getSprite());
    //collision.draw(window, sf::Color::Green);
}

const BoundingCircle& PlayerShoot::getBoundingCircle() const {
    return collision;
}

bool PlayerShoot::isOffScreen(const sf::Vector2u &screenSize) const {
    return (position.x < 0 || position.x > screenSize.x ||
            position.y < 0 || position.y > screenSize.y);
}

void PlayerShoot::setMovementStrategy(std::shared_ptr<MovementStrategy> strategy) {
    movementStrategy = std::move(strategy);
}

void PlayerShoot::cloneMovementStrategy() {
    if (movementStrategy)
        movementStrategy = movementStrategy->clone();
}

sf::Vector2f PlayerShoot::getDirection() { return direction; }