#include "../include/Bullet.h"
#include "../include/DirectionalMovement.h"
#include "../include/TurningMovement.h"
#include <iostream>

Bullet::Bullet() {}

Bullet::Bullet(SpriteTemplate spriteType, float speed, float acceleration, float velMin, float velMax)
    : position(0.f, 0.f),
    sprite(spriteType),
    dead(false)
{
    sprite.setPosition(position.x, position.y);
    float r = sprite.getSprite().getGlobalBounds().width / 2.f;
    collision = BoundingCircle(position, r);

    movementStrategy = std::make_shared<DirectionalMovement>(0.f, speed, acceleration, velMax, velMin);
}

Bullet::Bullet(SpriteTemplate spriteType, ColorType spriteColor, float speed, float acceleration, float velMin, float velMax)
    : position(0.f, 0.f),
    sprite(spriteType, spriteColor),
    dead(false)
{
    sprite.setPosition(position.x, position.y);
    float r = sprite.getSprite().getGlobalBounds().width / 2.f;
    collision = BoundingCircle(position, r);

    movementStrategy = std::make_shared<DirectionalMovement>(0.f, speed, acceleration, velMax, velMin);
}

Bullet::Bullet(SpriteTemplate spriteType, float initialDirDeg, float speed, float triggerTime, float degreesToTurn, float turnSpeed)
    : position(0.f, 0.f),
    sprite(spriteType),
    dead(false)
{
    sprite.setPosition(position.x, position.y);
    float r = sprite.getSprite().getGlobalBounds().width / 2.f;
    collision = BoundingCircle(position, r);

    // Usamos TurningMovement en lugar de DirectionalMovement
    movementStrategy = std::make_shared<TurningMovement>(initialDirDeg, speed, triggerTime, degreesToTurn, turnSpeed);
}

Bullet::Bullet(SpriteTemplate spriteType, ColorType spriteColor, float initialDirDeg, float speed, float triggerTime, float degreesToTurn, float turnSpeed)
    : position(0.f, 0.f),
    sprite(spriteType, spriteColor),
    dead(false)
{
    sprite.setPosition(position.x, position.y);
    float r = sprite.getSprite().getGlobalBounds().width / 2.f;
    collision = BoundingCircle(position, r);

    // Usamos TurningMovement en lugar de DirectionalMovement
    movementStrategy = std::make_shared<TurningMovement>(initialDirDeg, speed, triggerTime, degreesToTurn, turnSpeed);
}

void Bullet::update(float deltaTime) {
    if (dead) return;
    
    // Actualizar posición según estrategia de movimiento si existe
    if (movementStrategy) {
        movementStrategy->update(deltaTime);

        // Obtener el vector de movimiento actualizado
        sf::Vector2f movementVector = movementStrategy->getMovementVector();
        
        // Actualizar posición de la bala
        position += movementVector * deltaTime;

        // Calcular la dirección de rotación en base al vector
        if (movementVector.x != 0.f || movementVector.y != 0.f) {
            float angleRad = std::atan2(movementVector.y, movementVector.x);
            float angleDeg = angleRad * 180.f / 3.14159265f;

            // Aplicar la rotación al sprite
            sprite.setRotation(angleDeg + 90.f); // Ajuste para que apunte correctamente
        }
    }
    
    // Actualizar posición del sprite y colisión
    sprite.setPosition(position.x, position.y);
    collision.setCenter(position);
}

void Bullet::render(sf::RenderWindow &window) const {
    if (!dead) {
        window.draw(sprite.getSprite());
        // Para debug: mostrar círculo de colisión
        // collision.draw(window, sf::Color::Green);
    }
}

const BoundingCircle& Bullet::getBoundingCircle() const {
    return collision;
}

bool Bullet::isOffScreen(const sf::Vector2u &screenSize) const {
    float radius = collision.getRadius();
    return (position.x < -radius || position.x > screenSize.x + radius ||
            position.y < -radius || position.y > screenSize.y + radius);
}

bool Bullet::isDead() const {
    return dead;
}

void Bullet::setMovementStrategy(std::shared_ptr<MovementStrategy> strategy) {
    movementStrategy = std::move(strategy);
}

void Bullet::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(pos.x, pos.y);
    collision.setCenter(pos);
}

void Bullet::setDirection(const sf::Vector2f& dir) {
    if (!movementStrategy) return;

    float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159265f;
    movementStrategy->setDirection(angle);
}

void Bullet::cloneMovementStrategy() {
    if (movementStrategy)
        movementStrategy = movementStrategy->clone();
}

