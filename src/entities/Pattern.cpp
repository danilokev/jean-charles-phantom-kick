#include "../include/Pattern.h"
#include "../include/GamePlayState.h"

const sf::Vector2f UP_CENTER(320.f, 50.f);
const sf::Vector2f UP_CORNER_LEFT(64.f, 0.f);
const sf::Vector2f UP_CORNER_RIGHT(576.f, 0.f);

Pattern::Pattern()
    : spawnTimer(0.f), spawnInterval(1.f),
      totalToSpawn(5), spawnedCount(0),
      finished(false), spawnPosition(UP_CENTER)
{}

void Pattern::setEnemyTemplate(const Enemy& e) {
    enemyTemplate = e;
}

void Pattern::setSpawnInterval(float time) {
    spawnInterval = time;
    spawnTimer = time;
}

void Pattern::setNumberToSpawn(int count) {
    totalToSpawn = count;
}

void Pattern::setSpawnPosition(const sf::Vector2f& pos) {
    spawnPosition = pos;
}

bool Pattern::isFinished() const {
    return finished;
}

void Pattern::update(float deltaTime) {
    if (finished) return;

    spawnTimer += deltaTime;

    if (spawnedCount < totalToSpawn && spawnTimer >= spawnInterval) {
        spawnEnemy();
        spawnTimer = 0.f;
        spawnedCount++;

        if (spawnedCount >= totalToSpawn) {
            finished = true;
        }
    }
}

void Pattern::spawnEnemy() {
    Enemy newEnemy = enemyTemplate;
    newEnemy.cloneMovementStrategy(); // Muy importante

    newEnemy.setPosition(spawnPosition);
    GamePlayState::getInstance()->addEnemy(std::move(newEnemy));
}

