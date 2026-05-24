#include "../include/Bomb.h"

Bomb::Bomb()
    : currentRadius(0.f), maxRadius(0.f), expansionSpeed(0.f), active(false) {}

void Bomb::launch(const sf::Vector2f& origin, float maxRadius, float speed) {
    this->currentRadius = 0.f;
    this->maxRadius = maxRadius;
    this->expansionSpeed = speed;
    this->active = true;
    this->area.setCenter(origin);
    this->area.setRadius(0.f);
}

void Bomb::update(float deltaTime) {
    if (!active) return;

    currentRadius += expansionSpeed * deltaTime;
    area.setRadius(currentRadius);

    if (currentRadius >= maxRadius) {
        active = false;
    }
}

void Bomb::render(sf::RenderTarget& target) const {
    if (active) {
        area.draw(target, sf::Color::Cyan);  // color visible y suave
    }
}

void Bomb::applyTo(std::vector<Bullet>& bullets) {
    if (!active) return;

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& b) {
            return area.collidesWith(b.getBoundingCircle());
        }),
        bullets.end()
    );
}

bool Bomb::isActive() const {
    return active;
}
