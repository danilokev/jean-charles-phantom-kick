#ifndef BULLETSPAWNEFFECT_H
#define BULLETSPAWNEFFECT_H

#include <SFML/Graphics.hpp>

class BulletSpawnEffect {
public:
    BulletSpawnEffect(const sf::Vector2f& position);
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    bool isFinished() const;

private:
    sf::CircleShape waveShape;
    float currentSize;
    float maxSize;
    bool finished;
};

#endif