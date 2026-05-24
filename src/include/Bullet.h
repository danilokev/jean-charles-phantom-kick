#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "BoundingCircle.h"
#include "MovementStrategy.h"
#include <memory>


class Bullet {
public:
    Bullet();
    Bullet(SpriteTemplate spriteType, float speed, float acceleration, float velMin = 1.0f, float velMax = 999.0f);
    Bullet(SpriteTemplate spriteType, ColorType spriteColor, float speed, float acceleration, float velMin = 1.0f, float velMax = 999.0f);
    // Constructor para TurningMovement sin Color
    Bullet(SpriteTemplate spriteType, float initialDirDeg, float speed, float triggerTime, float degreesToTurn, float turnSpeed);
    // Constructor para TurningMovement con Color
    Bullet(SpriteTemplate spriteType, ColorType spriteColor, float initialDirDeg, float speed, float triggerTime, float degreesToTurn, float turnSpeed);

        
    void update(float deltaTime);
    void render(sf::RenderWindow &window) const;
    
    const BoundingCircle& getBoundingCircle() const;
    bool isOffScreen(const sf::Vector2u &screenSize) const;
    bool isDead() const;
    
    // Establecer estrategia de movimiento
    void setPosition(const sf::Vector2f& pos);
    void setDirection(const sf::Vector2f& dir);
    void getDirection() const;
    
    void cloneMovementStrategy();
    
    //Este método no se si usa,
    void setMovementStrategy(std::shared_ptr<MovementStrategy> strategy);
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    Sprite sprite;
    BoundingCircle collision;
    bool dead;
    std::shared_ptr<MovementStrategy> movementStrategy;
};

#endif