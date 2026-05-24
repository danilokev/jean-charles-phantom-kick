#include "../include/BulletEmitter.h"
#include "../include/GamePlayState.h"
#include "../include/DirectionalMovement.h"
#include <cmath>
#include <iostream>

BulletEmitter::BulletEmitter()
    : coneAngle(360.f),
      numBullets(1),
      direction(1.f, 0.f),
      bulletTracksPlayer(false)
{}

void BulletEmitter::resetTimer(float offset)
{
    timer = offset;
}

void BulletEmitter::emit(const sf::Vector2f& origin, const Bullet& templateBullet) {
    const float PI = 3.14159265f;

    // ------------------------------
    // Métodos Auxiliares
    // ------------------------------

    // Calcula la dirección base (con o sin tracking al jugador)
    auto computeBaseDirection = [&](sf::Vector2f baseDir) {
        if (bulletTracksPlayer) {
            sf::Vector2f playerPos = GamePlayState::getInstance()->getPlayerPosition();
            baseDir = revert ? (origin - playerPos) : (playerPos - origin);

            float mag = std::hypot(baseDir.x, baseDir.y);
            if (mag != 0.f) baseDir /= mag;
        }
        if (rotation != 0.f && !bulletTracksPlayer) {
            float angleRad = std::atan2(direction.y, direction.x);
            angleRad += rotation * PI / 180.0f;
            direction = sf::Vector2f(std::cos(angleRad), std::sin(angleRad));
        }
        return baseDir;
    };

    // Añade variación en el modo caos
    auto applyChaos = [&](sf::Vector2f& spawnPos, const sf::Vector2f& dir) {
        if (chaosMode) {
            float rx = ((float)rand() / RAND_MAX - 0.5f) * chaosWindow.x;
            float ry = ((float)rand() / RAND_MAX - 0.5f) * chaosWindow.y;

            sf::Vector2f forward = dir;
            sf::Vector2f right(-forward.y, forward.x);

            sf::Vector2f chaosOffset = right * rx + forward * ry;
            spawnPos += chaosOffset;
        }
    };

    // Spawnea una bala en la posición y dirección indicada
    auto spawnBullet = [&](const sf::Vector2f& spawnPos, const sf::Vector2f& dir) {
        Bullet b = templateBullet;
        b.cloneMovementStrategy();
        b.setDirection(dir);
        b.setPosition(spawnPos);
        GamePlayState::getInstance()->addBullet(std::move(b));
    };

    // ------------------------------
    // Modo Normal (sin ráfaga)
    // ------------------------------
    if (numBurst <= 1) {
        sf::Vector2f baseDir = computeBaseDirection(direction);

        float angleRad = std::atan2(baseDir.y, baseDir.x);
        float startAngle;
        if (bulletTracksPlayer && (numBullets > 1) && (numBullets % 2 == 1)) {
            // El centro está alineado al jugador, calculamos a los lados
            startAngle = angleRad - (coneAngle * PI / 180.0f) / 2.0f + (coneAngle * PI / 180.0f) / (numBullets - 1) / 2.0f;
        } else {
            // Modo normal
            startAngle = angleRad - (coneAngle * PI / 180.0f) / 2.0f;
        }
        
        float step = (numBullets > 1) ? (coneAngle * PI / 180.0f) / (numBullets) : 0.f;

        for (int i = 0; i < numBullets; ++i) {
            float angle = startAngle + i * step;
            sf::Vector2f dir(std::cos(angle), std::sin(angle));
            sf::Vector2f spawnPos = origin + dir * offsetDistance;
            applyChaos(spawnPos, dir);
            spawnBullet(spawnPos, dir);
        }

        timer = 0.f;
        return;
    }

    // ------------------------------
    // Modo Burst (ráfaga)
    // ------------------------------
    if (!inBurst) {
        burstCount = 0;
        burstTimer = 0.f;
        inBurst = true;

        burstDirection = computeBaseDirection(direction);
        burstPosition = origin;
        burstConeAngle = coneAngle;
        burstBulletNum = numBullets;
        burstOriginOffset = offsetDistance;
    }

    burstTimer += 1.f/60.f;

    if (burstCount < numBurst) {
        if (burstCount == 0 || burstTimer >= burstCooldown) {
            burstTimer = 0.f;

            float angleRad = std::atan2(burstDirection.y, burstDirection.x);
            angleRad += burstRotation * burstCount * PI / 180.0f;

            burstConeAngle += burstConeVariation * burstCount;

            float startAngle = angleRad - (burstConeAngle * PI / 180.0f) / 2.0f;
            float step = (burstBulletNum > 1) ? (burstConeAngle * PI / 180.0f) / (burstBulletNum) : 0.f;

            for (int i = 0; i < burstBulletNum; ++i) {
                float angle = startAngle + i * step;
                sf::Vector2f dir(std::cos(angle), std::sin(angle));
                sf::Vector2f spawnPos = burstPosition + dir * burstOriginOffset + burstOffset * static_cast<float>(burstCount);
                applyChaos(spawnPos, dir);
                spawnBullet(spawnPos, dir);
            }

            ++burstCount;
        }
    }

    if (burstCount >= numBurst) {
        inBurst = false;
        timer = 0.f;
    }
}

void BulletEmitter::update(float deltaTime) {
    timer += deltaTime;
}

bool BulletEmitter::canEmit() const {
    return timer >= cooldown;
}

// Configuración general
void BulletEmitter::setConeAngle(float angle) {
    coneAngle = angle;
}

void BulletEmitter::setNumBullets(int count) {
    numBullets = count;
}

void BulletEmitter::setDirection(const sf::Vector2f& dir) {
    direction = dir;
}

void BulletEmitter::setTrackPlayer(bool enable, bool revert) {
    bulletTracksPlayer = enable;
    this->revert = revert;
}

void BulletEmitter::setOffsetDistance(float d) {
    offsetDistance = d;
}

void BulletEmitter::setCooldown(float seconds) {
    cooldown = seconds;
}

void BulletEmitter::setRotation(float rot) {
    rotation = rot;
}

// Ráfaga (Burst)
void BulletEmitter::setNumBurst(int count) {
    numBurst = count;
}

void BulletEmitter::setBurstCooldown(float seconds) {
    burstCooldown = seconds;
}

void BulletEmitter::setBurstRotation(float degrees) {
    burstRotation = degrees;
}

void BulletEmitter::setBurstOffset(const sf::Vector2f& offset) {
    burstOffset = offset;
}

void BulletEmitter::setBurstConeVariation(float degrees) {
    burstConeVariation = degrees;
}

void BulletEmitter::setBurstBulletAdd(int count) {
    burstBulletAdd = count;
}

void BulletEmitter::setBurstOriginOffsetVariation(float d) {
    burstOriginOffsetVariation = d;
}

void BulletEmitter::setChaosMode(bool enable) {
    chaosMode = enable;
}

void BulletEmitter::setChaosWindow(const sf::Vector2f& window) {
    chaosWindow = window;
}

sf::Vector2f BulletEmitter::normalize(const sf::Vector2f& v) {
    float mag = std::sqrt(v.x * v.x + v.y * v.y);
    if (mag != 0.f)
        return v / mag;
    return {0.f, 1.f}; // fallback hacia abajo
}

#include "../include/BulletEmitter.h"

BulletEmitter BulletEmitter::clone() const {
    BulletEmitter copy;

    // Configuración General
    copy.coneAngle = this->coneAngle;
    copy.numBullets = this->numBullets;
    copy.direction = this->direction;
    copy.bulletTracksPlayer = this->bulletTracksPlayer;
    copy.offsetDistance = this->offsetDistance;
    copy.cooldown = this->cooldown;

    // No tocamos el timer, ese se resetea al clonar
    // copy.timer = this->timer;

    // Rotación del patrón
    copy.rotation = this->rotation;

    // Parámetros de Ráfaga (Burst)
    copy.numBurst = this->numBurst;
    copy.burstCooldown = this->burstCooldown;

    // No tocamos estos temporizadores
    // copy.burstTimer = this->burstTimer;
    // copy.inBurst = this->inBurst;
    // copy.burstCount = this->burstCount;

    copy.burstRotation = this->burstRotation;
    copy.burstOffset = this->burstOffset;
    copy.burstConeVariation = this->burstConeVariation;
    copy.burstBulletAdd = this->burstBulletAdd;
    copy.burstOriginOffsetVariation = this->burstOriginOffsetVariation;

    // Almacenamiento temporal durante burst
    copy.burstDirection = this->burstDirection;
    copy.burstPosition = this->burstPosition;
    copy.burstConeAngle = this->burstConeAngle;
    copy.burstBulletNum = this->burstBulletNum;
    copy.burstOriginOffset = this->burstOriginOffset;

    // Modo caos
    copy.chaosMode = this->chaosMode;
    copy.chaosWindow = this->chaosWindow;

    // Dirección inversa
    copy.revert = this->revert;

    return copy;
}
