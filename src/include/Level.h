#ifndef LEVEL_H
#define LEVEL_H

#include "Pattern.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Boss.h"
#include "TurningMovement.h"


struct ScheduledPattern {
    float startingTime;
    Pattern pattern;
};

struct Wave {
    std::vector<Pattern> patternPool;       // pool local de patrones
    std::vector<int> order;                 // índices dentro de patternPool
    std::vector<Pattern> activePatterns;    // patrones ya lanzados (vivos o no)

    float delayBetweenPatterns = 1.5f;
    size_t currentIndex = 0;
    float delayTimer = 0.f;
    bool finished = false;

    void update(float deltaTime) {
        // Activar siguiente patrón si corresponde
        if (currentIndex < order.size()) {
            delayTimer += deltaTime;
            if (delayTimer >= delayBetweenPatterns) {
                int idx = order[currentIndex];
                if (idx >= 0 && idx < patternPool.size()) {
                    activePatterns.push_back(patternPool[idx]);
                }
                currentIndex++;
                delayTimer = 0.f;
            }
        }

        // Actualizar todos los patrones activos (aunque estén terminados, no se borran aún)
        for (auto& p : activePatterns) {
            p.update(deltaTime);
        }

        // Condición de finalización: todos lanzados y todos terminados
        if (currentIndex >= order.size()) {
            finished = std::all_of(activePatterns.begin(), activePatterns.end(),
                                    [](const Pattern& p) { return p.isFinished(); });
        }
    }

    bool isFinished() const {
        return finished;
    }
};

class Level {
    public:
        Level();
        void reset();
        void genLevel1();
        void genLevel2();
        void genLevel3();
        void genLevel4();
        void genLevel5();

        void update(float deltaTime);
        bool isBossActive() const;
        Boss& getCurrentBoss();     // solo válido si isBossReady() es true
        bool levelIsComplete() const;
        std::string getLevelTilemap() const;
        bool isFinalBoss();
    
    private:
        std::vector<Wave> waves;
        size_t currentWaveIndex = 0;
        float waveDelayTimer = 0.f;
        float delayBetweenWaves = 2.0f;
        std::string tilemapTmxFile;

        Boss subBoss;
        Boss bossFinal;
        std::unique_ptr<Boss> currentBoss = nullptr;
        bool bossActive = false;
        bool bossDefeated = false;
        bool finalBossLaunched = false;

        bool hasSubBoss = false;
        bool subBossUsed = false;
        int subBossWaveIndex = -1;

        bool levelCompleted = false;

    
        void loadWave(const std::vector<Pattern>& pool, const std::vector<int>& order, float delay);

        std::vector<Pattern> babyAttack();
        std::vector<Pattern> secondAttack(int level);
        std::vector<Pattern> randomRainFlurry(int level);
        std::vector<Pattern> randomPeekingShoot(int level);
        std::vector<Pattern> spiralWave(int level);
        std::vector<Pattern> crimsonSpiral();
        std::vector<Pattern> ghostMarch();
        std::vector<Pattern> risingLance();
        std::vector<Pattern> syncopatedDagger();
        std::vector<Pattern> huntingAttack(float x);
        std::vector<Pattern> zigZagBarrage();
        std::vector<Pattern> crossFire();
        std::vector<Pattern> waveCrest();
        std::vector<Pattern> starBurst();
        std::vector<Pattern> pincerStrike();
        std::vector<Pattern> plungingAttack(float separation);
        std::vector<Pattern> curtainDrop();
        std::vector<Pattern> arcSwing();
        std::vector<Pattern> ambushRise();
        std::vector<Pattern> diagonalWeave();
        std::vector<Pattern> pulseWave();
        std::vector<Pattern> vortexRush();
        std::vector<Pattern> doubleSpiral();
        std::vector<Pattern> phantomStrike();
        std::vector<Pattern> advancedPincer();
    };
    

#endif