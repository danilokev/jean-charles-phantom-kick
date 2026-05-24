#include "../include/BulletSpawnEffect.h"
#include "../include/SoundPlayer.h"

BulletSpawnEffect::BulletSpawnEffect(const sf::Vector2f &position)
{
  currentSize = 5.f;
  maxSize = 30.f;
  waveShape.setPosition(position);
  waveShape.setOutlineColor(sf::Color::Cyan);
  waveShape.setOutlineThickness(2.f);
  waveShape.setFillColor(sf::Color::Transparent);
  finished = false;
}

void BulletSpawnEffect::update(float deltaTime) {
  if (currentSize < maxSize) {
    currentSize += 150.f * deltaTime;
    waveShape.setRadius(currentSize);
    waveShape.setOrigin(currentSize, currentSize);
    waveShape.setOutlineColor(sf::Color(0, 255, 255, static_cast<sf::Uint8>(255 - (255 * (currentSize / maxSize)))));
  } else {
    finished = true;
  }
}

void BulletSpawnEffect::render(sf::RenderWindow &window) const {
  if (!finished)
    window.draw(waveShape);
}

bool BulletSpawnEffect::isFinished() const {
  return finished;
}