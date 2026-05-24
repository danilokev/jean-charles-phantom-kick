    #include "../include/Level.h"
    #include "../include/DirectionalMovement.h"
    #include "../include/TurningMovement.h"
    #include "../include/MusicPlayer.h"
    #include <iostream>
    #include "../include/Level.h"

    const sf::Vector2f UP_AT_LEFT(96.f, 0.f);
    const sf::Vector2f UP_AT_RIGHT(360.f, 0.f);
    const sf::Vector2f LEFT_AT_UP(0.f, 96.f);
    const sf::Vector2f RIGHT_AT_UP(424.f, 96.f);
    const sf::Vector2f MIDDLE_UP(228.f, 0.f);
    const sf::Vector2f UP_CORNER_LEFT(64.f, 0.f);
    const sf::Vector2f UP_CORNER_RIGHT(576.f, 0.f);
    const sf::Vector2f DOWN_CORNER_LEFT(64.f, 480.f);
    const sf::Vector2f DOWN_CORNER_RIGHT(576.f, 480.f);

    Level::Level()
        : currentWaveIndex(0),
        waveDelayTimer(0.f),
        delayBetweenWaves(2.0f),
        bossActive(false),
        hasSubBoss(false),
        subBossUsed(false),
        subBossWaveIndex(-1),
        levelCompleted(false),
        currentBoss(nullptr)
    {}

    void Level::reset() {
        waves.clear();
        currentWaveIndex = 0;
        waveDelayTimer = 0.f;
        bossActive = false;
        finalBossLaunched = false;
        hasSubBoss = false;
        subBossUsed = false;
        subBossWaveIndex = -1;
        levelCompleted = false;
        tilemapTmxFile.clear();

        currentBoss.reset(); // Elimina jefe activo si lo hubiera
        bossFinal = Boss();  // Reinicia jefe final
        subBoss = Boss();    // Reinicia subboss
    }

    void Level::update(float deltaTime) {
        if (levelCompleted) {
            return;
        }

        // 1. Si hay boss activo, no actualizamos oleadas
        if (bossActive) {
            if (currentBoss) {
                currentBoss->update(deltaTime);
                if (currentBoss->bossIsDead()) {
                    bossActive = false;
                    currentBoss.reset();

                    if (bossFinal.bossIsDead()) {
                        std::cout << "[Level] Boss final ha muerto. Nivel completado.\n";
                        levelCompleted = true;
                    }
                }
            }
            return;
        }

        // 2. Actualizar oleada actual (si queda alguna)
        if (currentWaveIndex < waves.size()) {
            waves[currentWaveIndex].update(deltaTime);

            if (waves[currentWaveIndex].isFinished()) {
                // 3. Activar subboss si corresponde
                if (hasSubBoss && !subBossUsed && subBossWaveIndex == static_cast<int>(currentWaveIndex)) {
                    currentBoss = std::make_unique<Boss>(std::move(subBoss));
                    currentBoss->setStateActive();
                    bossActive = true;
                    subBossUsed = true;
                    return;
                }

                // 4. Esperar y pasar a la siguiente oleada
                waveDelayTimer += deltaTime;
                if (waveDelayTimer >= delayBetweenWaves) {
                    currentWaveIndex++;
                    waveDelayTimer = 0.f;
                }
            }

            return;
        }

        // 5. Si ya no quedan oleadas y no hemos lanzado el boss final
        if (!finalBossLaunched && !bossFinal.bossIsDead()) {
            currentBoss = std::make_unique<Boss>(std::move(bossFinal));
            currentBoss->setStateActive();
            bossActive = true;
            finalBossLaunched = true;
        }
    }

    bool Level::isFinalBoss() {
        return !finalBossLaunched;
    }
        
    void Boss::setStateActive() {
        state = BossState::Active;
    }

    Boss& Level::getCurrentBoss() {
        return *currentBoss;
    }

    bool Level::isBossActive() const {
        return currentBoss && bossActive;
    }

    bool Level::levelIsComplete() const {
        return levelCompleted;
    }

    void Level::loadWave(const std::vector<Pattern>& pool, const std::vector<int>& order, float delay) {
        Wave wave;
        wave.patternPool = pool;
        wave.order = order;
        wave.delayBetweenPatterns = delay;
        wave.delayTimer = delay;
        waves.push_back(wave);
    }

    std::string Level::getLevelTilemap() const {
        return tilemapTmxFile;
    }

    void Level::genLevel1() {
        
        tilemapTmxFile = "resources/tilesets/entrance.tmx";
        loadWave(babyAttack(), {1,0}, 4.f);
        loadWave(secondAttack(1), {1,0}, 4.f);
        loadWave(randomPeekingShoot(1), {0, 2, 7, 8, 3, 1, 3, 6, 7, 5, 1, 3, 4, 5, 1, 0}, 0.5f);
        loadWave(spiralWave(1), {0, 1}, 2.3f);
        loadWave(randomRainFlurry(1), {2, 2, 8, 8, 0, 0, 3, 3, 5}, 0.5f);
        loadWave(plungingAttack(0.f), {1, 0}, 0.f);  // Casi al centro
        loadWave(randomRainFlurry(1), {4}, 5.f);

        bossFinal = Boss().genBoss1();
    }
    void Level::genLevel2() 
    {
        tilemapTmxFile = "resources/tilesets/dirty_pool.tmx";

        // Oleada 1: Cortina de enemigos desde arriba
        loadWave(curtainDrop(), {0, 1, 2, 3, 4}, 1.0f);
        // Oleada 2: Arcos giratorios desde los lados
        loadWave(arcSwing(), {0, 1}, 2.0f);
        // Oleada 3: Emboscada desde abajo
        loadWave(ambushRise(), {0, 1, 2}, 1.5f);
        // Oleada 4: Tejido diagonal
        loadWave(diagonalWeave(), {0, 1, 2, 3}, 1.2f);
        // Oleada 5: Onda pulsante
        loadWave(pulseWave(), {0, 1}, 2.0f);
        // Oleada 6: Carga en vórtice
        loadWave(vortexRush(), {0, 1}, 2.5f);

        subBoss = Boss().genSubBoss2();
        hasSubBoss = true;
        subBossWaveIndex = 5;

        bossFinal = Boss().genBoss2();
    }
    void Level::genLevel3() {
        tilemapTmxFile = "resources/tilesets/corridor.tmx";

        loadWave(ghostMarch(), {0, 1}, 0.8f);
        loadWave(doubleSpiral(), {0, 1}, 2.0f);
        loadWave(phantomStrike(), {0, 1, 2}, 1.5f);
        loadWave(randomRainFlurry(4), {2, 5, 1, 7, 3, 6, 0, 8, 4}, 0.7f);
        loadWave(plungingAttack(80.f), {1, 0, 1}, 0.5f);
        loadWave(advancedPincer(), {0, 1}, 2.5f);

        hasSubBoss = false;

        bossFinal = Boss().genBoss3();
    }

    void Level::genLevel4() {
        tilemapTmxFile = "resources/tilesets/stairs.tmx";
        loadWave(randomRainFlurry(4), {0, 0, 1, 1, 2, 2, 3, 3}, 0.8f);
        loadWave(randomRainFlurry(4), {7, 7, 6, 6, 5, 5, 4, 4}, 0.8f);
        loadWave(spiralWave(4), {0, 1}, 5.f);
        loadWave(risingLance(), {0, 2, 1}, 3.f);
        loadWave(syncopatedDagger(), {0, 1}, 2.f);

        subBoss = Boss().genSubBoss4();
        hasSubBoss = true;
        subBossWaveIndex = 4;

        loadWave(starBurst(), {2, 1, 0, 3, 4, 3}, 2.5f);
        loadWave(waveCrest(), {1, 0, 0, 1}, 2.f);
        
        loadWave(plungingAttack(70.f), {1, 0, 0, 1}, 0.f);  // Casi al centro
        loadWave(plungingAttack(130.f), {1, 0}, 0.f);  // Más cerca
        loadWave(plungingAttack(190.f), {1, 0, 0, 1}, 0.f);  // Lejos del centro

        loadWave(randomRainFlurry(5), {4}, 0.8f);

        bossFinal = Boss().genBoss4();
    }

    void Level::genLevel5() {
        tilemapTmxFile = "resources/tilesets/main_stair.tmx";

        loadWave(randomRainFlurry(5), {4, 2, 8, 1, 0, 7, 3, 6, 5}, 1.f);
        loadWave(secondAttack(5), {0, 1}, 6.f);
        loadWave(spiralWave(5), {0, 1}, 2.3f);
        loadWave(zigZagBarrage(), {0, 1, 2, 3}, 1.5f);
        loadWave(crossFire(), {0, 1}, 3.f);
        loadWave(waveCrest(), {0, 1}, 2.f);
        loadWave(huntingAttack(228), {0, 1}, 0.f);
        loadWave(starBurst(), {0, 1, 2, 3, 4}, 2.5f);
        loadWave(pincerStrike(), {0, 1}, 2.f);
        
        loadWave(plungingAttack(190.f), {1, 0}, 0.f);  // Lejos del centro
        loadWave(plungingAttack(130.f), {1, 0}, 0.f);  // Más cerca
        loadWave(plungingAttack(70.f), {1, 0}, 0.f);  // Casi al centro

        bossFinal = Boss().genBoss5(); // SIEMPRE hay boss final
    }

    std::vector<Pattern> Level::babyAttack() {
        std::vector<Pattern> patterns;

        struct MirrorCfg {
            sf::Vector2f spawnPos;
            float turnDir;
        };

        const MirrorCfg cfgs[] = {
            { UP_AT_LEFT, -90.f },
            { UP_AT_RIGHT, 90.f }
        };

        const float spawnInterval = 0.4f;
        const unsigned numToSpawn = 8;
        const float enemyRadius = 10.f;
        const float initDirDeg = 90.f;
        const float velocity = 2.f;
        const float triggerTime = 1.2f;
        const float turnSpeedDeg = 180.f;
        const float enemyLife = 1.f;

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            BulletEmitter emitter;
            emitter.setNumBullets(0);
            Bullet bullet;
            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    // Nuevos patrones para el nivel 2
    std::vector<Pattern> Level::curtainDrop() {
        std::vector<Pattern> patterns;

        // Enemigos aparecen en una "cortina" desde la parte superior, espaciados uniformemente
        const float spawnXs[] = { 96.f, 172.f, 248.f, 324.f, 400.f };
        const float spawnInterval = 0.5f;
        const unsigned numToSpawn = 2;
        const float enemyRadius = 10.f;
        const float velocity = 3.0f;
        const float enemyLife = 2.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(2);
        emitter.setCooldown(1.0f);
        emitter.setConeAngle(30.f);

        Bullet bullet(KUNAI, BLUE, 120.f, 0.f);

        for (float x : spawnXs) {
            sf::Vector2f spawnPos(x, 0.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(90.f, velocity, 0.f);

            Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::arcSwing() {
        std::vector<Pattern> patterns;

        struct ArcCfg {
            sf::Vector2f spawnPos;
            float initDirDeg;
            float turnDir;
        };

        const ArcCfg cfgs[] = {
            { LEFT_AT_UP, 0.f, 90.f },   // Desde izquierda, gira hacia abajo
            { RIGHT_AT_UP, 180.f, -90.f } // Desde derecha, gira hacia abajo
        };

        const float spawnInterval = 0.6f;
        const unsigned numToSpawn = 4;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float triggerTime = 0.8f;
        const float turnSpeedDeg = 100.f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setCooldown(0.8f);
        emitter.setConeAngle(45.f);

        Bullet bullet(ROUND_BULLET, YELLOW, 130.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                c.initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::ambushRise() {
        std::vector<Pattern> patterns;

        const float spawnXs[] = { 120.f, 228.f, 336.f };
        const float spawnInterval = 0.7f;
        const unsigned numToSpawn = 3;
        const float enemyRadius = 12.f;
        const float velocity = 2.8f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(2);
        emitter.setCooldown(0.9f);
        emitter.setConeAngle(20.f);

        Bullet bullet(ESFERA, RED, 110.f, 0.f);

        for (float x : spawnXs) {
            sf::Vector2f spawnPos(x, 480.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(270.f, velocity, 0.f);

            Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_3, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::diagonalWeave() {
        std::vector<Pattern> patterns;

        struct WeaveCfg {
            sf::Vector2f spawnPos;
            float initDirDeg;
            float turnDir;
        };

        const WeaveCfg cfgs[] = {
            { sf::Vector2f(64.f, 0.f), 45.f, 45.f },    // Esquina superior izquierda
            { sf::Vector2f(400.f, 0.f), 135.f, -45.f },  // Esquina superior derecha, dentro del límite
            { sf::Vector2f(64.f, 480.f), 315.f, 45.f },  // Esquina inferior izquierda
            { sf::Vector2f(400.f, 480.f), 45.f, -45.f } // Esquina inferior derecha, dentro del límite
        };

        const float spawnInterval = 0.7f;
        const unsigned numToSpawn = 3;
        const float enemyRadius = 10.f;
        const float velocity = 2.8f;
        const float triggerTime = 0.6f;
        const float turnSpeedDeg = 90.f;
        const float enemyLife = 2.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(2);
        emitter.setCooldown(1.0f);
        emitter.setConeAngle(20.f);

        Bullet bullet(CARAMBANO, BLUE, 115.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                c.initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_4, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::pulseWave() {
        std::vector<Pattern> patterns;

        struct PulseCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const PulseCfg cfgs[] = {
            { sf::Vector2f(0.f, 120.f), 0.f },   // Desde izquierda, mueve hacia derecha
            { sf::Vector2f(424.f, 120.f), 180.f } // Desde derecha, mueve hacia izquierda
        };

        const float spawnInterval = 0.5f;
        const unsigned numToSpawn = 5;
        const float enemyRadius = 10.f;
        const float velocity = 3.0f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setCooldown(0.7f);
        emitter.setConeAngle(30.f);
        emitter.setNumBurst(2);
        emitter.setBurstCooldown(0.2f);

        Bullet bullet(BALA, ORANGE, 125.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::vortexRush() {
        std::vector<Pattern> patterns;

        struct VortexCfg {
            sf::Vector2f spawnPos;
            float initDirDeg;
            float turnDir;
        };

        const VortexCfg cfgs[] = {
            { MIDDLE_UP, 90.f, 180.f },  // Centro, gira en sentido horario
            { MIDDLE_UP, 90.f, -180.f }  // Centro, gira en sentido antihorario
        };

        const float spawnInterval = 0.4f;
        const unsigned numToSpawn = 6;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float triggerTime = 0.4f;
        const float turnSpeedDeg = 150.f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(4);
        emitter.setCooldown(0.6f);
        emitter.setConeAngle(60.f);

        Bullet bullet(ROUND_BULLET, PURPLE, 120.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                c.initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::secondAttack(int level) {
        std::vector<Pattern> patterns;

        struct MirrorCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const MirrorCfg cfgs[] = {
            { LEFT_AT_UP, 25.f },
            { RIGHT_AT_UP, 155.f }
        };

        const float spawnInterval = 1.f;
        const unsigned numToSpawn = 4;
        const float velocity = 2.f;
        const float enemyRadius = 10.f;
        const float enemyLife = 4.f;

        BulletEmitter baseEmitter;
        if (level != 5) baseEmitter.setTrackPlayer(true, true);
        else baseEmitter.setTrackPlayer(true);
        baseEmitter.setCooldown(1.f);
        if (level == 5) {
            baseEmitter.setCooldown(0.3f);
            baseEmitter.setNumBurst(6);
            baseEmitter.setConeAngle(1);
            baseEmitter.setBurstConeVariation(10);
        }

        Bullet baseBullet(ROUND_BULLET, 200.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, baseEmitter.clone(), baseBullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::randomRainFlurry(int level) {
        std::vector<Pattern> patterns;

        constexpr unsigned sideCount = 4;
        const float rightMin = 96.f;
        const float rightMax = 212.f;
        const float leftMin = 212.f;
        const float leftMax = 360.f;

        const float stepRight = (rightMax - rightMin) / (sideCount + 1);
        const float stepLeft = (leftMax - leftMin) / (sideCount + 1);

        std::vector<float> xs;
        for (unsigned i = 0; i < sideCount; ++i)
            xs.emplace_back(leftMax - (i + 1) * stepLeft);
        xs.emplace_back(MIDDLE_UP.x);
        for (unsigned i = 0; i < sideCount; ++i)
            xs.emplace_back(rightMax - (i + 1) * stepRight);

        const float spawnInterval = 1.f;
        const unsigned numToSpawn = 1;
        float velocity = 4.f;
        const float enemyRadius = 10.f;
        const float enemyLife = 1.f;

        if (level < 5) velocity = 8.f;

        BulletEmitter baseEmitter;
        baseEmitter.setTrackPlayer(true, true);
        if (level == 5) baseEmitter.setCooldown(0.1f);
        else if (level == 4) baseEmitter.setCooldown(0.6f);
        else baseEmitter.setCooldown(1.f);
        baseEmitter.setNumBullets(5);
        if (level == 5) baseEmitter.setNumBurst(3);
        else baseEmitter.setNumBurst(2);
        if (level == 5) baseEmitter.setBurstRotation(3.0);

        Bullet baseBullet(KUNAI, ORANGE, 70.f, 0.f);

        for (float x : xs) {
            sf::Vector2f spawnPos(x, 0.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(90.f, velocity, 0.f);

            Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_1, baseEmitter.clone(), baseBullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }


    std::vector<Pattern> Level::randomPeekingShoot(int level) {
        std::vector<Pattern> patterns;

        constexpr unsigned sideCount = 4;
        const float rightMin = 96.f;
        const float rightMax = 212.f;
        const float leftMin = 212.f;
        const float leftMax = 360.f;

        const float stepRight = (rightMax - rightMin) / (sideCount + 1);
        const float stepLeft = (leftMax - leftMin) / (sideCount + 1);

        std::vector<float> xs;
        for (unsigned i = 0; i < sideCount; ++i)
            xs.emplace_back(leftMax - (i + 1) * stepLeft);
        xs.emplace_back(MIDDLE_UP.x);
        for (unsigned i = 0; i < sideCount; ++i)
            xs.emplace_back(rightMax - (i + 1) * stepRight);

        const float spawnInterval = 1.f;
        const unsigned numToSpawn = 1;
        float velocity = 4.f;
        const float enemyRadius = 10.f;
        const float enemyLife = 1.f;

        if (level < 5) velocity = 4.f;

        BulletEmitter baseEmitter;
        baseEmitter.setConeAngle(180.f);
        baseEmitter.setTrackPlayer(true);
        if (level == 5) baseEmitter.setCooldown(0.1f);
        else if (level == 4) baseEmitter.setCooldown(0.6f);
        else baseEmitter.setCooldown(1.f);
        baseEmitter.setNumBullets(5);
        if (level == 5) baseEmitter.setNumBurst(3);
        else baseEmitter.setNumBurst(2);
        if (level == 5) baseEmitter.setBurstRotation(3.0);

        Bullet baseBullet(ESFERA, RED, 70.f, 0.f);

        for (float x : xs) {
            sf::Vector2f spawnPos(x, 0.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(90.f, velocity, -4.f, 10.f, -10.f);

            Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_4, baseEmitter.clone(), baseBullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::plungingAttack(float separation) {
        std::vector<Pattern> patterns;

        const float leftX  = MIDDLE_UP.x - separation;
        const float rightX = MIDDLE_UP.x + separation;

        const float spawnInterval  = 1.f;
        const unsigned numToSpawn  = 1;
        const float enemyRadius    = 10.f;
        const float enemyLife      = 20.f;
        const float enemySpeed     = 4.f;
        const float enemyAccel     = 2.f;

        Bullet bullet(ESFERA, RED, 0.f, 100.f);

        BulletEmitter emitter;
        emitter.setNumBullets(1);
        emitter.setTrackPlayer(true, true);
        emitter.setCooldown(0.1f);

        // Posiciones: izquierda, derecha
        std::vector<sf::Vector2f> positions = {
            {leftX, 16.f},
            {rightX, 16.f},
        };

        for (const auto& spawnPos : positions) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(
                90.f, enemySpeed, enemyAccel
            );

            Enemy enemy(spawnPos, std::move(movement),
                        enemyRadius,
                        ENEMY_3,
                        emitter,
                        bullet,
                        enemyLife);
            
            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }


    std::vector<Pattern> Level::huntingAttack(float x) {
        std::vector<Pattern> patterns;

        sf::Vector2f spawnPos(x, 0.f);

        const float spawnInterval = 1.f;
        const unsigned numToSpawn = 1;
        const float radius = 10.f;
        const float life = 100.f;

        // Movimiento compartido
        auto createMovement = []() {
            return std::make_unique<DirectionalMovement>(90.f, 8.f, -6.f, 99.f, -1.f);
        };

        // Primer enemigo: balas normales
        {
            BulletEmitter emitter;
            emitter.setTrackPlayer(false);
            emitter.setNumBullets(18);
            emitter.setCooldown(2.5f);

            Bullet bullet(BALA, ORANGE, 150.f, 0.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            Enemy enemy(MIDDLE_UP, createMovement(), radius, ENEMY_3, emitter.clone(), bullet, life);
            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        // Segundo enemigo: ráfaga de balas con aceleración
        {
            BulletEmitter emitter;
            emitter.setTrackPlayer(false);
            emitter.setNumBullets(18);
            emitter.setCooldown(1.5f);
            emitter.setNumBurst(6); // solo una ráfaga
            emitter.setRotation(45);

            Bullet bullet(CARAMBANO, BLUE, 0.f, 100.f);

            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            Enemy enemy(MIDDLE_UP, createMovement(), radius, ENEMY_3, emitter.clone(), bullet, life);
            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::spiralWave(int level) {
        std::vector<Pattern> patterns;

        struct SpiralCfg {
            sf::Vector2f spawnPos;
            float turnDir;
        };

        const SpiralCfg cfgs[] = {
            { MIDDLE_UP, -360.f },  // Giro en sentido horario
            { MIDDLE_UP, 360.f }  // Giro en sentido antihorario
        };

        const float spawnInterval = 0.5f;
        const unsigned numToSpawn = 6;
        const float enemyRadius = 10.f;
        const float velocity = 3.f;
        const float triggerTime = 0.5f;
        const float turnSpeedDeg = 120.f;
        const float enemyLife = 6.f;

        BulletEmitter emitter;
        if (level == 5) emitter.setNumBullets(4);
        if (level == 4) emitter.setNumBullets(3);
        else emitter.setNumBullets(2);
        if (level == 5) emitter.setDirection({0,1});
        else emitter.setTrackPlayer(true);
        emitter.setCooldown(0.8f);
        emitter.setConeAngle(90.f);
        

        Bullet bullet(ROUND_BULLET, PURPLE, 250.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                90.f, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::zigZagBarrage() {
        std::vector<Pattern> patterns;

        struct ZigZagCfg {
            sf::Vector2f spawnPos;
            float initDirDeg;
            float turnDir;
        };

        const ZigZagCfg cfgs[] = {
            { UP_AT_LEFT, 90.f, 45.f },   // Izquierda, zigzag hacia derecha
            { UP_AT_LEFT, 90.f, -45.f },  // Izquierda, zigzag hacia izquierda
            { UP_AT_RIGHT, 90.f, 45.f },  // Derecha, zigzag hacia derecha
            { UP_AT_RIGHT, 90.f, -45.f }  // Derecha, zigzag hacia izquierda
        };

        const float spawnInterval = 0.8f;
        const unsigned numToSpawn = 5;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float triggerTime = 1.f;
        const float turnSpeedDeg = 90.f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setCooldown(1.5f);
        emitter.setConeAngle(30.f);

        Bullet bullet(KUNAI, BLUE, 120.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                c.initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::crossFire() {
        std::vector<Pattern> patterns;

        struct CrossCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const CrossCfg cfgs[] = {
            { UP_CORNER_LEFT, 45.f },  // Diagonal desde esquina superior izquierda a inferior derecha
            { UP_CORNER_RIGHT, 135.f }  // Diagonal desde esquina superior derecha a inferior izquierda
        };

        const float spawnInterval = 1.2f;
        const unsigned numToSpawn = 4;
        const float enemyRadius = 12.f;
        const float velocity = 3.f;
        const float enemyLife = 4.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(5);
        emitter.setCooldown(0.6f);
        emitter.setConeAngle(45.f);

        Bullet bullet(ROUND_BULLET, RED, 150.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::waveCrest() {
        std::vector<Pattern> patterns;

        struct WaveCfg {
            sf::Vector2f spawnPos;
            float initDirDeg;
            float turnDir;
        };

        const WaveCfg cfgs[] = {
            { LEFT_AT_UP, 5.f, 90.f },  // Desde izquierda, oscilando hacia derecha
            { RIGHT_AT_UP, 175.f, -90.f } // Desde derecha, oscilando hacia izquierda
        };

        const float spawnInterval = 0.6f;
        const unsigned numToSpawn = 6;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float triggerTime = 0.5f;
        const float turnSpeedDeg = 90.f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setCooldown(0.5f);
        emitter.setConeAngle(20.f);

        Bullet bullet(KUNAI, GREEN, 130.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                c.initDirDeg, velocity, triggerTime, c.turnDir, turnSpeedDeg
            );

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::starBurst() {
        std::vector<Pattern> patterns;

        struct StarCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const StarCfg cfgs[] = {
            { MIDDLE_UP, 60.f },   // Diagonal ligeramente derecha
            { MIDDLE_UP, 90.f },   // Abajo
            { MIDDLE_UP, 120.f },  // Diagonal ligeramente izquierda
            { MIDDLE_UP, 30.f },   // Muy a la derecha
            { MIDDLE_UP, 150.f }   // Muy a la izquierda
        };

        const float spawnInterval = 0.7f;
        const unsigned numToSpawn = 3;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float enemyLife = 2.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setCooldown(1.4f);
        emitter.setConeAngle(60.f);

        Bullet bullet(ROUND_BULLET, YELLOW, 140.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::pincerStrike() {
        std::vector<Pattern> patterns;

        struct PincerCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const PincerCfg cfgs[] = {
            { sf::Vector2f(150.f, 480.f), 300.f },  // Desde izquierda inferior hacia centro superior
            { sf::Vector2f(490.f, 480.f), 240.f }   // Desde derecha inferior hacia centro superior
        };

        const float spawnInterval = 0.8f;
        const unsigned numToSpawn = 5;
        const float enemyRadius = 12.f;
        const float velocity = 2.7f;
        const float enemyLife = 4.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(4);
        emitter.setCooldown(0.6f);
        emitter.setConeAngle(90.f);

        Bullet bullet(ROUND_BULLET, BLUE, 120.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::crimsonSpiral() {
        std::vector<Pattern> patterns;

        const sf::Vector2f spawnPos = MIDDLE_UP;
        const float spawnInterval = 0.3f;
        const unsigned numToSpawn = 10;
        const float enemyRadius = 10.f;
        const float velocity = 2.5f;
        const float triggerTime = 0.4f;
        const float turnSpeedDeg = 150.f;
        const float enemyLife = 2.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(false);
        emitter.setNumBullets(6);
        emitter.setCooldown(0.2f);
        emitter.setConeAngle(60.f);

        Bullet bullet(ROUND_BULLET, RED, 110.f, 0.f);

        const float turnDirs[] = { 360.f, -360.f };

        for (float turnDir : turnDirs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(spawnPos);

            auto movement = std::make_unique<TurningMovement>(
                90.f, velocity, triggerTime, turnDir, turnSpeedDeg
            );

            Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::ghostMarch() {
        std::vector<Pattern> patterns;

        struct MarchCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const MarchCfg cfgs[] = {
            { sf::Vector2f(0.f, 100.f), 0.f },
            { sf::Vector2f(424.f, 100.f), 180.f }
        };

        const float spawnInterval = 0.6f;
        const unsigned numToSpawn = 8;
        const float enemyRadius = 10.f;
        const float velocity = 2.f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(2);
        emitter.setCooldown(0.8f);
        emitter.setConeAngle(20.f);

        Bullet bullet(KUNAI, GREEN, 100.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::risingLance() {
        std::vector<Pattern> patterns;

        const float spawnYs[] = { 480.f };
        const float spawnXs[] = { 96.f, 212.f, 360.f };

        const float spawnInterval = 0.7f;
        const unsigned numToSpawn = 3;
        const float enemyRadius = 12.f;
        const float velocity = 2.8f;
        const float enemyLife = 3.f;

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(4);
        emitter.setCooldown(0.5f);
        emitter.setConeAngle(30.f);

        Bullet bullet(ROUND_BULLET, YELLOW, 130.f, 0.f);

        for (float y : spawnYs) {
            for (float x : spawnXs) {
                sf::Vector2f spawnPos(x, y);

                Pattern pattern;
                pattern.setSpawnInterval(spawnInterval);
                pattern.setNumberToSpawn(numToSpawn);
                pattern.setSpawnPosition(spawnPos);

                auto movement = std::make_unique<DirectionalMovement>(270.f, velocity, 0.f);

                Enemy enemy(spawnPos, std::move(movement), enemyRadius, ENEMY_2, emitter.clone(), bullet, enemyLife);

                pattern.setEnemyTemplate(enemy);
                patterns.emplace_back(std::move(pattern));
            }
        }

        return patterns;
    }

    std::vector<Pattern> Level::syncopatedDagger() {
        std::vector<Pattern> patterns;

        struct DaggerCfg {
            sf::Vector2f spawnPos;
            float dirDeg;
        };

        const DaggerCfg cfgs[] = {
            { sf::Vector2f(0.f, 150.f), 0.f },
            { sf::Vector2f(424.f, 150.f), 180.f }
        };

        const float spawnInterval = 0.8f;
        const unsigned numToSpawn = 6;
        const float enemyRadius = 10.f;
        const float velocity = 3.5f;
        const float enemyLife = 2.f;

        BulletEmitter emitter;
        emitter.setRotation(37.f);
        emitter.setOffsetDistance(20.f);
        emitter.setNumBullets(3);
        emitter.setCooldown(0.3f);
        emitter.setConeAngle(15.f);

        Bullet bullet(KUNAI, PURPLE, 140.f, 0.f);

        for (const auto& c : cfgs) {
            Pattern pattern;
            pattern.setSpawnInterval(spawnInterval);
            pattern.setNumberToSpawn(numToSpawn);
            pattern.setSpawnPosition(c.spawnPos);

            auto movement = std::make_unique<DirectionalMovement>(c.dirDeg, velocity, 0.f);

            Enemy enemy(c.spawnPos, std::move(movement), enemyRadius, ENEMY_1, emitter.clone(), bullet, enemyLife);

            pattern.setEnemyTemplate(enemy);
            patterns.emplace_back(std::move(pattern));
        }

        return patterns;
    }

    std::vector<Pattern> Level::doubleSpiral() {
        std::vector<Pattern> patterns;
        const sf::Vector2f spawnPos = MIDDLE_UP;
        const float velocities[] = {2.8f, 3.2f};

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(4);
        emitter.setConeAngle(45.f);
        emitter.setCooldown(0.6f);

        Bullet bullet(CARAMBANO, BLUE, 140.f, 0.f);

        for (float vel : velocities) {
            for (float dir : {-180.f, 180.f}) {
                Pattern pattern;
                auto movement = std::make_unique<TurningMovement>(
                    90.f, vel, 0.4f, dir, 130.f);

                Enemy enemy(spawnPos, std::move(movement), 10.f, ENEMY_2,
                            emitter.clone(), bullet, 4.f);

                pattern.setSpawnInterval(0.45f);
                pattern.setNumberToSpawn(10);
                pattern.setSpawnPosition(spawnPos);
                pattern.setEnemyTemplate(enemy);
                patterns.push_back(pattern);
            }
        }
        return patterns;
    }

    std::vector<Pattern> Level::phantomStrike() {
        std::vector<Pattern> patterns;
        const sf::Vector2f positions[] = {
            {64.f, 0.f}, {424.f, 0.f}, {228.f, 480.f}};

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(3);
        emitter.setConeAngle(60.f);
        emitter.setCooldown(0.8f);
        emitter.setBurstCooldown(0.3f);

        Bullet bullet(KUNAI, PURPLE, 160.f, 0.f);

        for (auto &pos : positions) {
            Pattern pattern;
            auto movement = std::make_unique<DirectionalMovement>(
                (pos.y == 480.f) ? 270.f : 90.f,
                4.5f,
                -3.f,
                15.f,
                -15.f);

            Enemy enemy(pos, std::move(movement), 14.f, ENEMY_4,
                        emitter.clone(), bullet, 5.f);

            pattern.setSpawnInterval(1.0f);
            pattern.setNumberToSpawn(4);
            pattern.setSpawnPosition(pos);
            pattern.setEnemyTemplate(enemy);
            patterns.push_back(pattern);
        }
        return patterns;
    }

    std::vector<Pattern> Level::advancedPincer() {
        std::vector<Pattern> patterns;
        const sf::Vector2f positions[] = {
            {0.f, 240.f}, {424.f, 240.f}};

        BulletEmitter emitter;
        emitter.setTrackPlayer(true);
        emitter.setNumBullets(4);
        emitter.setConeAngle(90.f);
        emitter.setCooldown(0.5f);

        Bullet bullet(ROUND_BULLET, YELLOW, 180.f, 0.f);

        for (auto &pos : positions) {
            Pattern pattern;
            float angle = (pos.x == 0) ? 45.f : 135.f;

            auto movement = std::make_unique<TurningMovement>(
                angle, 3.8f, 1.0f, (pos.x == 0) ? 30.f : -30.f, 80.f);

            Enemy enemy(pos, std::move(movement), 12.f, ENEMY_2,
                        emitter.clone(), bullet, 6.f);

            pattern.setSpawnInterval(0.6f);
            pattern.setNumberToSpawn(6);
            pattern.setSpawnPosition(pos);
            pattern.setEnemyTemplate(enemy);
            patterns.push_back(pattern);
        }
        return patterns;
    }