#ifndef ENEMYDEATHEFFECT_H
#define ENEMYDEATHEFFECT_H

#include <SFML/Graphics.hpp>
#include <vector>

struct Particle
{
  sf::Vector2f position;
  sf::Vector2f velocity;
  float lifetime;
};

class EnemyDeathEffect
{
public:
  EnemyDeathEffect(const sf::Vector2f &position);
  void update(float deltaTime);
  void render(sf::RenderWindow &window) const;
  bool isFinished() const;

private:
  std::vector<Particle> particles;
  sf::Clock timer;
  bool finished;
};

#endif