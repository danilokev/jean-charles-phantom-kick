#include "../include/Enemy.h"
#include "../include/RenderManager.h"

Enemy::Enemy() {}

Enemy::Enemy(sf::Vector2f p,
    std::shared_ptr<MovementStrategy> m,
    float collisionRadius,
    SpriteTemplate spriteType,
    BulletEmitter be,
    Bullet b,
    float lp)
    : position(p),
    movement(std::move(m)),
    collision(p, collisionRadius),
    sprite(spriteType),
    spawn(std::move(be)),          // copia propia del emitter
    bulletToSpawn(std::move(b)),
    lifePoints(lp),
    isDead(false)
    {
        sprite.setPosition(position.x, position.y);
        spawn.resetTimer();
    }


void Enemy::update(float delta) {
    // Actualizamos la lógica de movimiento
    movement->update(delta);
    sf::Vector2f moveVec = movement->getMovementVector();
    position += moveVec;

    // Actualizamos el bounding circle y la posición del sprite
    collision.setCenter(position);
    sprite.setPosition(position.x, position.y);

    sprite.updateAnimation(delta); // actualiza la animación del sprite

    spawn.update(delta); // actualiza su temporizador interno

    if (spawn.canEmit()) {
        spawn.emit(position, bulletToSpawn);
    }

    // Marcar como muerto si se sale de la pantalla
    sf::RenderWindow* window = RenderManager::getInstance()->getWindow();
    if (window) {
        sf::Vector2u size = window->getSize();
        float r = collision.getRadius();
        if (position.x + r < 0 || position.x - r > size.x ||
            position.y + r < 0 || position.y - r > size.y) {
            isDead = true;
        }
    }
}

bool Enemy::isOffScreen(const sf::Vector2u &screenSize) const {
    float radius = collision.getRadius();
    return (position.x < -radius || position.x > (screenSize.x-208) + radius ||
            position.y < -radius || position.y > screenSize.y + radius);
}

void Enemy::cloneMovementStrategy() {
    if (movement)
        movement = movement->clone();
}


void Enemy::render() const {
    // Dibujamos el sprite del enemigo
    RenderManager::getInstance()->draw(sprite.getSprite());
}

void Enemy::setPosition(sf::Vector2f pos) {
    position = pos;
}

sf::Vector2f Enemy::getPosition() const {
    return position; 
}

bool Enemy::enemyIsDead() const {
    return isDead;
}

const BoundingCircle& Enemy::getBoundingCircle() const {
    return collision;
}

void Enemy::takeDamage(float damage) {
    lifePoints -= damage;
    if (lifePoints <= 0) {
        isDead = true;
    }
}