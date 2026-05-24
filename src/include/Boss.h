#ifndef BOSS_H
#define BOSS_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "BulletEmitter.h"
#include "MovementStrategy.h"
#include "Sprite.h"
#include "BoundingCircle.h"
#include "RenderManager.h"
#include "Bomb.h"
#include "DirectionalMovement.h"
#include "TurningMovement.h"


// Un spawner es un emitter que puede moverse
struct BulletSpawner {
    BulletEmitter emitter;
    std::shared_ptr<MovementStrategy> movement;
    Bullet bulletTemplate;
    sf::Vector2f position;
    bool followsBoss = false;
    bool isDead = false;

    BulletSpawner(const BulletEmitter& e, std::shared_ptr<MovementStrategy> m, const Bullet& b, bool followBoss = false)
        : emitter(e), movement(std::move(m)), bulletTemplate(b), position(0.f, 0.f), followsBoss(followBoss) {}

    /** @brief Actualiza el spawner de forma independiente. */
    void update(float deltaTime) {
        emitter.update(deltaTime);
    }

    /** @brief Actualiza el spawner siguiendo la posición del jefe. */
    void updateFollowingBoss(float deltaTime, const sf::Vector2f& bossPosition) {
        position = bossPosition;
        emitter.update(deltaTime);
    }

    /** @brief Emite balas si puede. */
    void emitBullets() {
        if (emitter.canEmit()) {
            emitter.emit(position, bulletTemplate);
        }
    }

    void setPosition(sf::Vector2f p) {position = p;}
};

class Boss {
private:
    sf::Vector2f position;
    Sprite sprite;
    BoundingCircle collision;
    std::shared_ptr<MovementStrategy> movement;

    int maxHealthBars;
    int currentHealthBar;
    float maxLifePerBar;
    float currentLife;

    enum class BossState { Inactive, Active, Dead, Transitioning, WaitingForNext };

    BossState state = BossState::Inactive;

    bool triggerBomb = false;

    struct Phase {std::vector<BulletSpawner> spawners;};

    std::vector<Phase> phases;
    size_t currentPhaseIndex;

    // Para la barra de vida
    sf::Texture lifeBarTexture;
    sf::Sprite lifeBarSprite;
    sf::Vector2f lifeBarPosition;
    sf::Vector2f lifeBarSize;

    // Solo accesible internamente
    Boss(sf::Vector2f p, std::shared_ptr<MovementStrategy> m, float collisionRadius, SpriteTemplate spriteType, int healthBars, float lifePerBar);

    void setLifeBarTexture(const sf::Texture& texture);
    void addPhase(const std::vector<BulletSpawner>& spawners);

    void updateSpawners(float deltaTime);

public:
    Boss();
    void update(float deltaTime);
    void render() const;

    void handlePhaseTransition(float deltaTime);

    void takeDamage(float damage);
    bool bossIsDead() const;

    const BoundingCircle& getBoundingCircle() const;
    const sf::Vector2f getPosition() const;

    bool immune = false;
    sf::Vector2f targetPosition;
    float phaseTransitionSpeed = 100.f;

    bool shouldTriggerBomb() const;
    void resetBombTrigger();
    bool isWaitingForNext() const;

    static Boss genBoss1();
    static Boss genBoss2();
    static Boss genSubBoss2();
    static Boss genBoss3();
    static Boss genBoss4();
    static Boss genSubBoss4();
    static Boss genBoss5();
    void setStateActive();

    size_t getCurrentPhaseIndex() const; // Nuevo método
};

#endif