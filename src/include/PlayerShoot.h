#ifndef PLAYER_SHOOT_H
#define PLAYER_SHOOT_H

#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "BoundingCircle.h"
#include "MovementStrategy.h"

class PlayerShoot {
public:
    // Constructor: se le pasa la posición de origen, la dirección y la velocidad deseada.
    PlayerShoot(const sf::Vector2f &startPos, const int shootType, const sf::Vector2f &direction, float speed, float damageValue);

    // Actualiza la posición del proyectil según su velocidad y el deltaTime
    void update(float deltaTime);

    // Dibuja el proyectil sobre la ventana
    void render(sf::RenderWindow &window) const;

    // Devuelve su BoundingCircle para colisiones
    const BoundingCircle& getBoundingCircle() const;

    // Indica si el proyectil se ha salido de la pantalla (para su eliminación)
    bool isOffScreen(const sf::Vector2u &screenSize) const;

    float getDamage() const { return damage; }
    void setMovementStrategy(std::shared_ptr<MovementStrategy> strategy);
    void cloneMovementStrategy();
    sf::Vector2f getDirection();

private:
    sf::Vector2f position; // Posición actual del proyectil
    sf::Vector2f velocity; // Vector de velocidad (ya calculado como dirección normalizada * speed)
    sf::Vector2f direction;
    Sprite sprite;         // Sprite del proyectil (se usa el template BALA1, por ejemplo)
    BoundingCircle collision; // Para las colisiones
    std::shared_ptr<MovementStrategy> movementStrategy;
    float damage;
};

#endif 
