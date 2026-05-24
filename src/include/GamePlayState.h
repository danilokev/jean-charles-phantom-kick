#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <memory>
#include "DialogSistem.h"
#include "DialogData.h"

#include "GameState.h"
#include "DialogSystem.h"   
#include "TileMap.h"
#include "StateManager.h"
#include "Level.h"
#include "Player.h"
#include "PlayerShoot.h"
#include "ScreenFlashEffect.h"
#include "BulletSpawnEffect.h"
#include "EnemyDeathEffect.h"
#include "PlayerDeathEffect.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Level.h"
#include "Bomb.h"
#include "Item.h"
#include "LevelSelectState.h"

#define kVel 3.15f

struct Punto {
    sf::Sprite sprite;
    bool collected = false;
    float alpha = 255;
};

class GamePlayState : public GameState {
private:
    static GamePlayState* instance;
    GamePlayState();

    bool practiceMode = false;
    bool levelContinue = false;
    int currentLevelIndex;

    int currentScore = 0;
    Player player;

    std::vector<BulletSpawnEffect> bulletSpawnEffects;
    std::vector<EnemyDeathEffect> enemyDeathEffects;
    std::vector<PlayerDeathEffect> playerDeathEffects;
    ScreenFlashEffect flash;

    TileMap tilemap;
    std::vector<Item> items;

    std::vector<Punto> puntos;
    sf::Font font;
    sf::Text counterText;
    sf::Text puntosText;
    sf::Text highscoreText;
    sf::Text scoreText;

    sf::Clock clock;
    std::vector<PlayerShoot> bullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Enemy> enemies;
    Level level;
    bool goToNextLevel = false;
    Bomb bomb;

    // Sistema de diálogos
    std::unique_ptr<DialogSystem> dialog;
    bool dialogActive     = false;
    bool prevBossActive   = false;
    bool wasBossActive    = false;
    size_t prevPhaseIndex = 0;

    // Diálogo del boss
    std::unique_ptr<DialogSistem> bossDialog; 
    

    size_t nextDialogIndex= 0;
    std::vector<std::vector<std::string>> levelDialogs;
    std::vector<std::vector<std::string>> levelDialogImages;

    bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    void printDebugInfo(int nBullets, int nEnemy, int nPlayerShot);

    float rightPanelWidth;
    sf::Texture rightPanelTexture;
    sf::Sprite rightPanelSprite;

    sf::Texture topBottomTexture;
    sf::Sprite topSprite;
    sf::Sprite bottomSprite;
    sf::Texture leftPanelTexture;
    sf::Sprite leftPanelSprite;

    // Iconos para vidas y bombas
    sf::Texture heartTexture;
    sf::Texture starTexture;
    std::vector<sf::Sprite> lifeSprites;
    std::vector<sf::Sprite> bombSprites;

    void spawnItemOnEnemyDeath(const sf::Vector2f& position);
    void updateLifeSprites();
    void updateBombSprites();
    void nextLevel();

public:
    static GamePlayState* getInstance() {
        if (!instance) instance = new GamePlayState();
        return instance;
    }

    void addEnemy(Enemy&& e);
    void addBullet(Bullet&& b);
    sf::Vector2f getPlayerPosition() const;
    void updateBullets(float deltaTime);
    void setSelectedWeapon(int weaponIndex);
    void setPlayerLivesAndBombs(int lives, int bombsPerLife);

    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
    void updateItems(float deltaTime);
    void addItem(Item item);

    void practiceModeStart(int index);
    void resetGame();
};

#endif