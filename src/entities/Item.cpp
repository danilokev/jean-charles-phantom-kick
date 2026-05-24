#include "../include/Item.h"
#include "../include/RenderManager.h"
#include <SFML/Graphics.hpp>
#include "../include/Sprite.h"


Item::Item(sf::Vector2f position, ItemType type)
    : position(position), type(type), collected(false),
      collision(position, 16.0f), sprite(mapItemTypeToSpriteType(type)) {  // Ajusta el radio de colisión si es necesario
    movement = std::make_shared<DirectionalMovement>(90.f, -10.f, 3.f);;
}

SpriteTemplate Item::mapItemTypeToSpriteType(ItemType type) {
    switch (type) {
        case ItemType::PowerUp:
            return SpriteTemplate::POWER_UP;
        case ItemType::LifeUp:
            return SpriteTemplate::LIFE_UP;
        case ItemType::Points:
            return SpriteTemplate::POINTS;
        default:
            throw std::runtime_error("Tipo de Item no válido");
    }
}

void Item::update(float delta) {
    // Actualización del movimiento
    movement->update(delta);
    sf::Vector2f moveVec = movement->getMovementVector();
    position += moveVec;

    // Actualización del sprite y del círculo de colisión
    sprite.setPosition(position.x, position.y);
    collision.setCenter(position);

    // Comprobación de si está fuera de la pantalla
    sf::RenderWindow* window = RenderManager::getInstance()->getWindow();
    if (window) {
        sf::Vector2u size = window->getSize();
        if (isOffScreen(size)) collected = true;
    }
}

void Item::render() const {
    RenderManager::getInstance()->draw(sprite.getSprite());
}

bool Item::isOffScreen(const sf::Vector2u& screenSize) const {
    float radius = collision.getRadius();
    
    // Solo eliminamos si se sale por los lados (izquierda, derecha) o por abajo
    return (position.x < -radius || position.x > screenSize.x + radius ||
            position.y > screenSize.y + radius); // Eliminamos solo si se va hacia abajo
}

const BoundingCircle& Item::getBoundingCircle() const {
    return collision;
}

sf::Vector2f Item::getPosition() const {
    return position;
}

bool Item::isCollected() const {
    return collected;
}

const ItemType Item::getItemType() const {
    return type;
}