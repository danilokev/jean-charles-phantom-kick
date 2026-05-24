#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include "BoundingCircle.h"
#include "InputMovement.h"
#include "PlayerShoot.h"
#include "Sprite.h"
#include "SoundPlayer.h"
#include "MovementStrategy.h"

class Player {
 private:
  sf::Vector2f position;
  sf::Vector2f lastDirection;
  sf::Clock shootClock;
  sf::Clock invincibilityClock;
  float invincibilityTime = 2.0f;
  float shootCooldown = 0.1f;

  InputMovement movement;
  BoundingCircle collision;
  BoundingCircle itemCollision;
  Sprite sprite;
  Sprite soulSprite;

  int lives;
  bool isInvincible;
  int selectedWeapon;
  int powerUps;

 public:
  Player(const sf::Vector2f &startPosition);
  void update(float delta, int input);
  void render() const;
  sf::Vector2f getPosition() const { return position; }
  void setPosition(const sf::Vector2f &pos);
  const sf::Sprite &getSprite() const;
  sf::Vector2f getLastDirection() const { return lastDirection; }
  std::vector<PlayerShoot> shoot(const sf::Vector2f &direction);
  bool canShoot() const;
  void increaseLives(int cant = 1);
  void decreaseLives(int cant = 1);
  int getLives() const;
  const BoundingCircle& getBoundingCircle() const;
  const BoundingCircle& getItemsBoundingCircle() const;
  void setSelectedWeapon(int weaponIndex);
  void addPowerUp(bool max = false);
  int getPowerUp();
  void useBomb();
  bool playerIsInvincible();
};

#endif