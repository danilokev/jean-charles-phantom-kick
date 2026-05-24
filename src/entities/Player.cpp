#include "../include/Player.h"
#include "../include/PlayerShoot.h"
#include "../include/RenderManager.h"

Player::Player(const sf::Vector2f& startPosition)
    : position(startPosition),
      movement(400.0f),
      collision(startPosition, 2.f),
      itemCollision(startPosition, 12.f),
      sprite(PLAYER),
      lives(10),
      isInvincible(false),
      selectedWeapon(0),
      powerUps(2),
      soulSprite(FANTASMA_ANIMADO, RED)
{
    sprite.setPosition(position.x, position.y);
    soulSprite.setPosition(position.x, position.y);
    soulSprite.setScale(0.5f, 0.5f);
}

void Player::update(float delta, int /*input*/) {
    if (isInvincible && invincibilityClock.getElapsedTime().asSeconds() >= invincibilityTime) {
        isInvincible = false;
    }
  
    movement.update(delta);
    sf::Vector2f moveVec = movement.getMovementVector();

    bool isShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    if (isShiftPressed) {
        moveVec *= 0.5f;
    }

    position += moveVec;
    lastDirection = moveVec; // Update lastDirection
    collision.setCenter(position);
    itemCollision.setCenter(position);
    sprite.setPosition(position.x, position.y);
    soulSprite.setPosition(position.x, position.y);

    if (std::abs(moveVec.x) > std::abs(moveVec.y)) {
        if (moveVec.x > 0) {
            sprite.setFrame(1);
        } else if (moveVec.x < 0) {
            sprite.setFrame(2);
        }
    } else {
        if (moveVec.y > 0) {
            sprite.setFrame(0);
        } else if (moveVec.y < 0) {
            sprite.setFrame(3);
        }
    }
}

void Player::render() const {
    if (!isInvincible || (int)(invincibilityClock.getElapsedTime().asSeconds() * 10) % 2 == 0) {
        RenderManager::getInstance()->draw(sprite.getSprite());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
        if (sf::RenderWindow* win = RenderManager::getInstance()->getWindow()) {
            RenderManager::getInstance()->draw(soulSprite.getSprite());
        }
    }
}

void Player::setPosition(const sf::Vector2f& pos) {
    position = pos;
    collision.setCenter(pos);
    sprite.setPosition(pos.x, pos.y);
}

const sf::Sprite& Player::getSprite() const {
    return sprite.getSprite();
}

std::vector<PlayerShoot> Player::shoot(const sf::Vector2f& direction) {
    float bulletSpeed = 900.f;
    float baseDamage = 1.0f;
    shootClock.restart();

    std::vector<PlayerShoot> bullets;
    const float OFFSET_RATIO = 0.20f;
    float offsetX = sprite.getSprite().getGlobalBounds().width * OFFSET_RATIO;

    switch (selectedWeapon) {
        case 0: // Disparo 1: Standard dual shot
            baseDamage = 1.0f;
            bullets.emplace_back(sf::Vector2f(position.x - offsetX, position.y), 0, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x + offsetX, position.y), 0, direction, bulletSpeed, baseDamage);
            break;
        case 1: // Disparo 2: Wide area
            baseDamage = 0.3f;
            offsetX *= 2;
            bullets.emplace_back(sf::Vector2f(position.x - offsetX, position.y), 1, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x + offsetX, position.y), 1, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x - offsetX*2, position.y), 1, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x + offsetX*2, position.y), 1, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x, position.y), 1, direction, bulletSpeed, baseDamage);
            break;
        case 2: // Disparo 3: Wide spread
            baseDamage = 0.6f;
            bullets.emplace_back(sf::Vector2f(position.x, position.y), 2, sf::Vector2f(-0.2f, -1.f), bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x, position.y), 2, direction, bulletSpeed, baseDamage);
            bullets.emplace_back(sf::Vector2f(position.x, position.y), 2, sf::Vector2f(0.2f, -1.f), bulletSpeed, baseDamage);
            break;
        case 3: // Disparo 4: Heavy single shot
            baseDamage = 2.0f;
            bullets.emplace_back(sf::Vector2f(position.x, position.y), 3, direction, bulletSpeed * 0.5f, baseDamage);
            break;
    }

    if (powerUps == 1) {
        for (auto& bullet : bullets) {
            bullet = PlayerShoot(bullet.getBoundingCircle().getCenter(), selectedWeapon,
                                    bullet.getDirection(), 
                                    bulletSpeed, bullet.getDamage() * 1.5f);
        }
    }
    if (powerUps == 2) {
        bullets.emplace_back(sf::Vector2f(position.x, position.y), 5, sf::Vector2f(1.f, -1.f), bulletSpeed, baseDamage);
        bullets.emplace_back(sf::Vector2f(position.x, position.y), 5, sf::Vector2f(-1.f, -1.f), bulletSpeed, baseDamage);
    }

    return bullets;
}

bool Player::canShoot() const {
    float adjustedCooldown = shootCooldown;
        if (selectedWeapon == 3) {
            adjustedCooldown = shootCooldown*2;
        }
        if (powerUps == 3) {
            adjustedCooldown *= 0.7f;
        }
    return shootClock.getElapsedTime().asSeconds() >= adjustedCooldown;
}

void Player::increaseLives(int cant) {
    lives += cant;
}

void Player::decreaseLives(int cant) {
    if (!isInvincible) {
        lives = std::max(lives - cant, 0);
        isInvincible = true;
        invincibilityClock.restart();
    }
}

int Player::getLives() const {
    return lives;
}

const BoundingCircle& Player::getBoundingCircle() const {
    return collision;
}

const BoundingCircle& Player::getItemsBoundingCircle() const {
    return itemCollision;
}

void Player::setSelectedWeapon(int weaponIndex) {
    selectedWeapon = weaponIndex;
}

void Player::addPowerUp(bool max) {
    if (max) powerUps = 3;
    if (powerUps == 3) {
        return;
    }
    else powerUps++;
}

int Player::getPowerUp() { return powerUps; }

void Player::useBomb() {powerUps--;}

bool Player::playerIsInvincible() { return isInvincible; }