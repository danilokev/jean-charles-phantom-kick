#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "RenderManager.h"
#include "DirectionalMovement.h"
#include "BoundingCircle.h"
#include "Sprite.h"
#include <memory>

enum class ItemType {
    PowerUp,
    LifeUp,
    Points
};

class Item {
public:
    Item(sf::Vector2f position, ItemType type);

    void update(float delta);
    void render() const;
    bool isOffScreen(const sf::Vector2u& screenSize) const;
    const BoundingCircle& getBoundingCircle() const;
    sf::Vector2f getPosition() const;
    bool isCollected() const;
    const ItemType getItemType() const;

private:
    sf::Vector2f position;
    ItemType type;
    bool collected;
    BoundingCircle collision;
    Sprite sprite;
    std::shared_ptr<DirectionalMovement> movement;
    SpriteTemplate mapItemTypeToSpriteType(ItemType type);
};

#endif // ITEM_H
