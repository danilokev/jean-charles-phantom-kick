#include "../include/PlayerDeathEffect.h"
#include "../include/SoundPlayer.h"

PlayerDeathEffect::PlayerDeathEffect(const sf::Vector2f &position)
{
  currentRadius = 10.f;
  maxRadius = 50.f;
  fadeSpeed = 200.f;
  effectColor = sf::Color(255, 0, 0, 200);
  effectShape.setPosition(position);
  effectShape.setFillColor(effectColor);
  finished = false;
}

void PlayerDeathEffect::update(float deltaTime)
{
  if (currentRadius < maxRadius)
  {
    currentRadius += fadeSpeed * deltaTime;
    effectShape.setRadius(currentRadius);
    effectShape.setOrigin(currentRadius, currentRadius);
    effectColor.a = static_cast<sf::Uint8>(200 - (200 * (currentRadius / maxRadius)));
    effectShape.setFillColor(effectColor);
  }
  else
  {
    finished = true;
  }
}

void PlayerDeathEffect::render(sf::RenderWindow &window) const
{
  if (!finished)
    window.draw(effectShape);
}

bool PlayerDeathEffect::isFinished() const
{
  return finished;
}