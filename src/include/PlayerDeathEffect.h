#ifndef PLAYERDEATHEFFECT_H
#define PLAYERDEATHEFFECT_H

#include <SFML/Graphics.hpp>

class PlayerDeathEffect
{
public:
  PlayerDeathEffect(const sf::Vector2f &position);
  void update(float deltaTime);
  void render(sf::RenderWindow &window) const;
  bool isFinished() const;

private:
  sf::CircleShape effectShape;
  float currentRadius;
  float maxRadius;
  float fadeSpeed;
  sf::Color effectColor;
  bool finished;
};

#endif