#ifndef PATTERN_H
#define PATTERN_H

#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Pattern {
public:
    Pattern();

    void update(float deltaTime);
    bool isFinished() const;

    void setEnemyTemplate(const Enemy& e);
    void setSpawnInterval(float time);
    void setNumberToSpawn(int count);
    void setSpawnPosition(const sf::Vector2f& pos);

private:
    Enemy enemyTemplate;

    float spawnTimer;
    float spawnInterval;
    int totalToSpawn;
    int spawnedCount;
    bool finished;

    sf::Vector2f spawnPosition;

    void spawnEnemy(); // Crea una instancia del template y la pasa a GamePlayState
};

#endif
