#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "Sprite.h"
#include "BoundingCircle.h"
#include "MovementStrategy.h"
#include "TurningMovement.h"
#include "BulletEmitter.h"
#include "Bullet.h"

class Enemy {
private:
    sf::Vector2f position;
    std::shared_ptr<MovementStrategy> movement;
    BoundingCircle collision;
    Sprite sprite;

    BulletEmitter spawn;
    Bullet bulletToSpawn;

    float lifePoints;
    bool isDead;

public:
    Enemy();
    Enemy(sf::Vector2f p, std::shared_ptr<MovementStrategy> m, float collisionRadius,
    SpriteTemplate spriteType, BulletEmitter be, Bullet b, float lp);

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    void cloneMovementStrategy();
    bool isOffScreen(const sf::Vector2u &screenSize) const;

    void update(float delta);
    void render() const;
    bool enemyIsDead() const;
    const BoundingCircle& getBoundingCircle() const;
    void takeDamage(float damage);
};

#endif