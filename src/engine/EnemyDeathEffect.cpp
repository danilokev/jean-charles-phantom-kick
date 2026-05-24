#include "../include/EnemyDeathEffect.h"
#include "../include/SoundPlayer.h"
#include <cstdlib>

EnemyDeathEffect::EnemyDeathEffect(const sf::Vector2f &position)
{
  // Inicializar 20 partículas
  for (int i = 0; i < 20; ++i)
  {
    Particle p;
    p.position = position;
    p.velocity = sf::Vector2f((rand() % 200 - 100), (rand() % 200 - 100));
    p.lifetime = 1.0f;
    particles.push_back(p);
  }
  finished = false;
  SoundPlayer::getInstance().playSound("enemy_death");
}

void EnemyDeathEffect::update(float deltaTime)
{
  for (auto &p : particles)
  {
    p.position += p.velocity * deltaTime;
    p.lifetime -= deltaTime;
  }
  if (timer.getElapsedTime().asSeconds() >= 1.0f)
    finished = true;
}

void EnemyDeathEffect::render(sf::RenderWindow &window) const
{
  sf::CircleShape particleShape(3.f);
  for (const auto &p : particles)
  {
    if (p.lifetime > 0)
    {
      particleShape.setPosition(p.position);
      particleShape.setFillColor(sf::Color(255, 165, 0, static_cast<sf::Uint8>(255 * p.lifetime)));
      window.draw(particleShape);
    }
  }
}

bool EnemyDeathEffect::isFinished() const
{
  return finished;
}