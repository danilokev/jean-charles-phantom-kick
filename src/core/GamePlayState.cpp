#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <filesystem>
#include <memory>

#include "../include/DialogSistem.h"
#include "../include/DialogData.h"
#include "../include/Bullet.h"
#include "../include/GamePlayState.h"
#include "../include/MenuState.h"
#include "../include/GameOverState.h"
#include "../include/GameCompletedState.h"
#include "../include/PauseState.h"
#include "../include/Player.h"
#include "../include/RenderManager.h"
#include "../include/MusicPlayer.h"

static bool fileExists(const std::string& path)
{
    std::ifstream f(path.c_str(), std::ios::binary);
    return f.good();
}

std::vector<int> loadCSV(const std::string& path) {
    std::ifstream file(path);
    std::string line, word;
    std::vector<int> mapData;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        while (std::getline(ss, word, ',')) {
            if (!word.empty() &&
                word.find_first_not_of(" \t\n\r") != std::string::npos)
            {
                mapData.push_back(std::stoi(word));
            }
        }
    }
    return mapData;
}

GamePlayState* GamePlayState::instance = nullptr;

GamePlayState::GamePlayState()
 : player({160.f, 240.f}),
   // dialogActive(false),
   prevBossActive(false),
   prevPhaseIndex(0),
   currentScore(0),
   rightPanelWidth(224.f)
{
    // bossDialog = std::make_unique<DialogSystem>(640, 480, font);
    resetGame();

    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }

    if (!rightPanelTexture.loadFromFile("resources/side_menu.png"))
        std::cerr << "Error cargando la textura del panel derecho.\n";
    rightPanelSprite.setTexture(rightPanelTexture);
    rightPanelSprite.setPosition(640 - rightPanelWidth, 0);
    rightPanelSprite.setScale(rightPanelWidth / rightPanelTexture.getSize().x, 1.f);

    if (!topBottomTexture.loadFromFile("resources/arribayabajo.png"))
        std::cerr << "Error cargando la textura superior e inferior.\n";
    topSprite.setTexture(topBottomTexture);
    topSprite.setPosition(0, 0);
    bottomSprite.setTexture(topBottomTexture);
    bottomSprite.setPosition(0, 480 - topBottomTexture.getSize().y);

    if (!leftPanelTexture.loadFromFile("resources/izquierda.png"))
        std::cerr << "Error cargando la textura del panel izquierdo.\n";
    leftPanelSprite.setTexture(leftPanelTexture);
    leftPanelSprite.setPosition(0, topBottomTexture.getSize().y);

    if (!heartTexture.loadFromFile("resources/heart.png"))
        std::cerr << "Error cargando la textura del corazón.\n";
    if (!starTexture.loadFromFile("resources/star.png"))
        std::cerr << "Error cargando la textura de la estrella.\n";

    updateLifeSprites();
    updateBombSprites();

    highscoreText.setFont(font);
    highscoreText.setCharacterSize(18);
    highscoreText.setFillColor(sf::Color::White);
    highscoreText.setOutlineColor(sf::Color::Black);
    highscoreText.setOutlineThickness(1.f);


    //highscoreText.setString("Highscore   1092301100");
    int highscore = 0;
    std::ifstream scoreFile("scores.txt");
    if (scoreFile.is_open()) {
        int score;
        while (scoreFile >> score) {
            highscore = std::max(highscore, score);
        }
        scoreFile.close();
    }
    auto paddedHighscore = std::to_string(highscore);
    paddedHighscore.insert(paddedHighscore.begin(), 10 - paddedHighscore.size(), '0');
    highscoreText.setString("Highscore   " + paddedHighscore);


    scoreText = highscoreText;
    scoreText.setString("Score  0000000000");

    counterText.setFont(font);
    counterText.setCharacterSize(30);
    counterText.setFillColor(sf::Color::White);
    counterText.setOutlineColor(sf::Color::Black);
    counterText.setOutlineThickness(1.f);

    puntosText = counterText;
    puntosText.setFillColor(sf::Color::Yellow);
    bossDialog = std::make_unique<DialogSistem>(640, 480, font);

    clock.restart();
}

void GamePlayState::resetGame() {
    currentScore = 0;
    dialogActive = false;
    prevBossActive = false;
    wasBossActive = false;
    prevPhaseIndex = 0;
    currentLevelIndex = 0;
    goToNextLevel = false;
    
    // Limpiar entidades del juego
    bullets.clear();
    enemyBullets.clear();
    enemies.clear();
    items.clear();
    
    level.reset();
    if (!practiceMode) level.genLevel1();
    tilemap.load(level.getLevelTilemap());
    
    practiceMode = false;

    updateLifeSprites();
    updateBombSprites();

    scoreText.setString("Score  0000000000");
    clock.restart();
}


//jc
void GamePlayState::practiceModeStart(int index) {
    
    std::cout << "Practica";
    practiceMode = true;
    resetGame();
    currentLevelIndex = index;

    switch (index) {
        case 0: level.genLevel1(); break;
        case 1: level.genLevel2(); break;
        case 2: level.genLevel3(); break;
        case 3: level.genLevel4(); break;
        case 4: level.genLevel5(); break;
        default:
            std::cerr << "Índice de nivel no válido para el modo práctica.\n";
            break;
    }
    tilemap.load(level.getLevelTilemap());
    updateLifeSprites();
    updateBombSprites();
    scoreText.setString("Score  0000000000");
    clock.restart();
}

void GamePlayState::nextLevel() {
    MusicPlayer::getInstance().stop();

    std::string trackName = "level" + std::to_string(currentLevelIndex + 1) + "_track";
    MusicPlayer::getInstance().playTrack(trackName);

    if (practiceMode) {
        MusicPlayer::getInstance().stop();
        GameOverState::getInstance()->setScore(currentScore);
        StateManager stateManager;
        stateManager.changeState(GameOverState::getInstance());
        return;
    }

    dialogActive = false;
    prevBossActive = false;
    wasBossActive = false;
    goToNextLevel = false;

    bullets.clear();
    enemyBullets.clear();
    enemies.clear();
    items.clear();

    level.reset();
    currentLevelIndex++;

    /*
    if (currentLevelIndex >= 4) {
        MusicPlayer::getInstance().stop();
        GameCompletedState::getInstance()->setScore(currentScore);
        StateManager stateManager;
        stateManager.changeState(GameCompletedState::getInstance());
        return;
    }
    */


    if (currentLevelIndex >= 5) {
        std::cout << "Todos los niveles completados.\n";
        // Cambiar a estado de fin del juego si es necesario
        return;
    }

    switch (currentLevelIndex) {
        case 1: level.genLevel2(); break;
        case 2: level.genLevel3(); break;
        case 3: level.genLevel4(); break;
        case 4: level.genLevel5(); break;
        default:
            std::cerr << "Índice de nivel inválido en nextLevel().\n";
            break;
    }
    tilemap.load(level.getLevelTilemap());
    updateLifeSprites();
    updateBombSprites();
    clock.restart();
}


bool GamePlayState::checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}

void GamePlayState::setPlayerLivesAndBombs(int lives, int bombsPerLife) {
    player.increaseLives(lives - player.getLives());
    while (player.getPowerUp() < bombsPerLife) {
        player.addPowerUp();
    }
    while (player.getPowerUp() > bombsPerLife) {
        player.useBomb();
    }
    updateLifeSprites();
    updateBombSprites();
}

void GamePlayState::handleInput(sf::RenderWindow& window, StateManager& manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            SoundPlayer::getInstance().playSound("pause_sound");
            MusicPlayer::getInstance().pause();
            manager.changeState(PauseState::getInstance());
        }
    }
}

void GamePlayState::update(sf::RenderWindow& window, StateManager& manager) {
    const float dt = 1.f/60.f;
    player.update(dt, 0);

    if (level.isBossActive()) {
        level.getCurrentBoss().update(dt);
        if (level.getCurrentBoss().shouldTriggerBomb()) {
            bomb.launch(level.getCurrentBoss().getPosition(), 500, 500);
            level.getCurrentBoss().resetBombTrigger();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && !bomb.isActive() && player.getPowerUp() > 0 && !dialogActive) {
        SoundPlayer::getInstance().playSound("player_bomb");
        bomb.launch(player.getPosition());
        player.useBomb();
        updateBombSprites();
    }

    {
        sf::Vector2f pos = player.getPosition();
        float minX = 35.f, maxX = (640 * 2 / 3.f - 75.f) + 35.f;
        float minY = 35.f, maxY = (480 - 75.f) + 35.f;
        if (pos.x < minX) pos.x = minX;
        if (pos.x > maxX) pos.x = maxX;
        if (pos.y < minY) pos.y = minY;
        if (pos.y > maxY) pos.y = maxY;
        player.setPosition(pos);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && player.canShoot() && !dialogActive) {
        SoundPlayer::getInstance().playSound("bullet_spawn");
        auto nuevos = player.shoot({0.f,-1.f});
        bullets.insert(bullets.end(), nuevos.begin(), nuevos.end());
        bulletSpawnEffects.emplace_back(player.getPosition());
    }

    level.update(dt);
    for (auto& e : enemies) e.update(dt);
    for (auto& i : items) i.update(dt);

    bool nowBossActive = level.isBossActive();

    // 1) **PRE-BOSS**: acaba de ACTIVARSE el boss
    if (!wasBossActive && nowBossActive && !dialogActive && !level.isFinalBoss()) {
        auto& entries = DialogData::get(currentLevelIndex, Stage::PreBoss);
        for (auto& e : entries)
        bossDialog->addEntry(e.imagePath, e.text, e.alignRight);
        bossDialog->start();
        dialogActive = true;
        bullets.clear();
        enemyBullets.clear();
        enemies.clear();
        wasBossActive = true;
    }
    // 2) **Mientas haya diálogo activo**, pausamos la lógica de juego
    if (dialogActive) {
        bullets.clear();
        enemyBullets.clear();
        bossDialog->update();         // avanza con Z
        if (!bossDialog->isActive()) {
            dialogActive = false;
        }
    
        // ✅ MUY IMPORTANTE: actualizar el estado previo incluso en medio de diálogo
        prevBossActive = nowBossActive;
        return;
    }

    if (goToNextLevel && !dialogActive) {
        if (currentLevelIndex >= 4) {
            MusicPlayer::getInstance().stop();
            GameCompletedState::getInstance()->setScore(currentScore);
            manager.changeState(GameCompletedState::getInstance());
            return;
        }
        nextLevel();
    }

    if (level.isBossActive()) {
        size_t currentPhaseIndex = level.getCurrentBoss().getCurrentPhaseIndex();
        if (currentPhaseIndex > prevPhaseIndex) {
            player.addPowerUp();
            SoundPlayer::getInstance().playSound("boss_phase");
            updateBombSprites();
            prevPhaseIndex = currentPhaseIndex;
        }
    }

    // 3) **POST-BOSS**: acaba de MORIR el boss
    if (wasBossActive && !nowBossActive && !dialogActive  && !level.isFinalBoss()) {
        auto& entries = DialogData::get(currentLevelIndex, Stage::PostBoss);
        for (auto& e : entries)
        bossDialog->addEntry(e.imagePath, e.text, e.alignRight);
        bossDialog->start();
        dialogActive = true;
        prevBossActive = false;
        wasBossActive = false;
        goToNextLevel = true;
        // Aquí podrías, por ejemplo, incrementar vidas, puntuación, etc.
    }
        
    bomb.update(dt);
    bomb.applyTo(enemyBullets);
    updateBullets(dt);

    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dt);
        if (it->isOffScreen(window.getSize())) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = enemyBullets.begin(); it != enemyBullets.end();) {
        if (!it->isDead() && !player.playerIsInvincible() && it->getBoundingCircle().collidesWith(player.getBoundingCircle())) {
            player.decreaseLives(1);
            player.addPowerUp(true);
            playerDeathEffects.emplace_back(player.getPosition());
            updateLifeSprites();
            updateBombSprites();

            if (player.getLives() <= 0) {
                SoundPlayer::getInstance().playSound("player_death");
                GameOverState::getInstance()->setScore(currentScore);
                manager.changeState(GameOverState::getInstance());
                return;
            }
            it = enemyBullets.erase(it);
        } else {
            ++it;
        }
    }

    for (auto bit = bullets.begin(); bit != bullets.end();) {
        bool destroyed = false;
        for (auto eit = enemies.begin(); eit != enemies.end() && !destroyed;) {
            if (eit->isOffScreen(window.getSize())) {
                eit = enemies.erase(eit);
                continue;
            }
    
            if (bit->getBoundingCircle().collidesWith(eit->getBoundingCircle())) {
                eit->takeDamage(bit->getDamage());
                if (eit->enemyIsDead()) {
                    enemyDeathEffects.emplace_back(eit->getPosition());
                    spawnItemOnEnemyDeath(eit->getPosition());
                    eit = enemies.erase(eit);
                } else {
                    ++eit;
                }
                destroyed = true;
            } else {
                ++eit;
            }
        }
        if (!destroyed && level.isBossActive() &&
            bit->getBoundingCircle().collidesWith(level.getCurrentBoss().getBoundingCircle()))
        {
            level.getCurrentBoss().takeDamage(bit->getDamage());
            destroyed = true;
        }

        if (destroyed) {
            bit = bullets.erase(bit);
        } else {
            ++bit;
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Enemy& e){ return e.enemyIsDead(); }),
        enemies.end());

    flash.update(dt);
    for (auto& fx : bulletSpawnEffects) fx.update(dt);
    bulletSpawnEffects.erase(
        std::remove_if(bulletSpawnEffects.begin(), bulletSpawnEffects.end(),
                       [](auto& e){ return e.isFinished(); }),
        bulletSpawnEffects.end());
    for (auto& fx : enemyDeathEffects) fx.update(dt);
    enemyDeathEffects.erase(
        std::remove_if(enemyDeathEffects.begin(), enemyDeathEffects.end(),
                       [](auto& e){ return e.isFinished(); }),
        enemyDeathEffects.end());
    for (auto& fx : playerDeathEffects) fx.update(dt);
    playerDeathEffects.erase(
        std::remove_if(playerDeathEffects.begin(), playerDeathEffects.end(),
                       [](auto& e){ return e.isFinished(); }),
        playerDeathEffects.end());

    for (auto it = items.begin(); it != items.end();) {
        if (!it->isCollected() && it->getBoundingCircle().collidesWith(player.getItemsBoundingCircle())) {
            switch (it->getItemType()) {
                case ItemType::PowerUp:
                    SoundPlayer::getInstance().playSound("item_powerup");
                    player.addPowerUp();
                    updateBombSprites();
                    break;
                case ItemType::LifeUp:
                    player.increaseLives(1);
                    updateLifeSprites();
                    break;
                case ItemType::Points:
                    SoundPlayer::getInstance().playSound("item_powerup");
                    currentScore += 100;
                    scoreText.setString("Score " + std::to_string(currentScore));
                    break;
            }
            it = items.erase(it);
        } else {
            ++it;
        }
    }

    tilemap.update(1.f/60.f);

    auto paddedScore = std::to_string(currentScore);
    paddedScore.insert(paddedScore.begin(), 10 - paddedScore.size(), '0');
    scoreText.setString("Score  " + paddedScore);

    std::string highscoreStr = highscoreText.getString().toAnsiString();
    int highscore = std::stoi(highscoreStr.substr(12));

    if (currentScore > highscore) {
        auto paddedHighscore = std::to_string(currentScore);
        paddedHighscore.insert(paddedHighscore.begin(), 10 - paddedHighscore.size(), '0');
        highscoreText.setString("Highscore   " + paddedHighscore);
    }
}

void GamePlayState::render(sf::RenderWindow& window) {
    RenderManager* renderManager = RenderManager::getInstance();

    sf::RectangleShape mainPanel(sf::Vector2f(640 - rightPanelWidth, 480));
    mainPanel.setFillColor(sf::Color(30, 30, 30));
    renderManager->draw(mainPanel);

    renderManager->draw(tilemap);

    player.render();

    bomb.render(window);

    for (auto& e : enemies) e.render();

    for (const auto& b : enemyBullets) b.render(window);

    for (auto& bullet : bullets) bullet.render(window);

    for (const auto& item : items) {item.render();}

    for (const auto& effect : bulletSpawnEffects) effect.render(window);

    for (const auto& effect : enemyDeathEffects) effect.render(window);

    for (const auto& effect : playerDeathEffects) effect.render(window);

    renderManager->draw(leftPanelSprite);
    renderManager->draw(topSprite);
    renderManager->draw(bottomSprite);

    renderManager->draw(rightPanelSprite);

    float rightPanelX = 640 - rightPanelWidth;

    highscoreText.setPosition(rightPanelX + 20, 50);
    scoreText.setPosition(rightPanelX + 45, 80);

    renderManager->draw(highscoreText);
    renderManager->draw(scoreText);

    for (const auto& sprite : lifeSprites) {
        renderManager->draw(sprite);
    }
    for (const auto& sprite : bombSprites) {
        renderManager->draw(sprite);
    }

    printDebugInfo(enemyBullets.size(), enemies.size(), bullets.size());

    counterText.setPosition(rightPanelX + 10, 10);
    puntosText.setPosition(rightPanelX + 10, 200);
    renderManager->draw(counterText);
    renderManager->draw(puntosText);

    if (level.isBossActive()) {
        level.getCurrentBoss().render();
    } 

    if (flash.isActive()) window.draw(flash.getOverlay());

    if (dialogActive) {
        bossDialog->render(window);
      }

    window.display();
}

void GamePlayState::spawnItemOnEnemyDeath(const sf::Vector2f& position) {
    // Generador aleatorio
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Determinar la cantidad de ítems a spawnear
    int roll = std::rand() % 100;
    int itemCount;

    if (roll < 30) {
        itemCount = 1;
    } else if (roll < 55) {
        itemCount = 2;
    } else if (roll < 75) {
        itemCount = 3;
    } else if (roll < 90) {
        itemCount = 4;
    } else {
        itemCount = 5;
    }

    int spawnWindow = (itemCount - 1) * 20; // 0, 10, 20, 30, 40

    std::uniform_real_distribution<float> offsetDist(-spawnWindow / 2.f, spawnWindow / 2.f);

    for (int i = 0; i < itemCount; ++i) {
        // Posición aleatoria dentro de la ventana
        sf::Vector2f offset(offsetDist(gen), offsetDist(gen));
        sf::Vector2f itemPos = position + offset;

        // Determinar tipo de ítem individual
        int random = std::rand() % 100;
        ItemType itemType;
        if (random < 1) {
            itemType = ItemType::LifeUp;
        } else if (random < 6) {
            itemType = ItemType::PowerUp;
        } else {
            itemType = ItemType::Points;
        }

        Item newItem(itemPos, itemType);
        addItem(std::move(newItem));
    }
}


void GamePlayState::addBullet(Bullet&& b) { enemyBullets.push_back(std::move(b)); }

sf::Vector2f GamePlayState::getPlayerPosition() const {
    return player.getPosition();
}

void GamePlayState::updateBullets(float deltaTime) {
    for (auto& b : enemyBullets) b.update(deltaTime);

    sf::Vector2u screenSize = RenderManager::getInstance()->getWindow()->getSize();
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(), 
                       [&](const Bullet& b) { return b.isDead() || b.isOffScreen(screenSize); }),
        enemyBullets.end());
}

void GamePlayState::updateItems(float deltaTime) {
    for (auto& item : items) {
        item.update(deltaTime);
    }

    sf::Vector2u screenSize = RenderManager::getInstance()->getWindow()->getSize();
    items.erase(
        std::remove_if(items.begin(), items.end(), 
                       [&](const Item& item) { return item.isCollected() || item.isOffScreen(screenSize); }),
        items.end());
}

void GamePlayState::addItem(Item item) {
    items.push_back(std::move(item));
}

void GamePlayState::addEnemy(Enemy&& e) { enemies.push_back(std::move(e)); }

void GamePlayState::printDebugInfo(int nBullets, int nEnemies, int nPlayerShot) {
    auto formatText = [this](sf::Text& text, const std::string& content, float x, float y) {
        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1.f);
        text.setString(content);
        text.setPosition(x, y);
    };

    sf::Text textBullets;
    sf::Text textEnemies;
    sf::Text textShots;

    formatText(textBullets, "Enemy Bullets: " + std::to_string(nBullets), 416 + 20, 190);
    formatText(textEnemies, "Enemies: " + std::to_string(nEnemies), 416 + 45, 210);
    formatText(textShots, "Player Shots: " + std::to_string(nPlayerShot), 416 + 20, 230);

    RenderManager::getInstance()->draw(textBullets);
    RenderManager::getInstance()->draw(textEnemies);
    RenderManager::getInstance()->draw(textShots);
}

void GamePlayState::setSelectedWeapon(int weaponIndex) {
    player.setSelectedWeapon(weaponIndex);
}

void GamePlayState::updateLifeSprites() {
    lifeSprites.clear();
    int lives = player.getLives();
    lives = std::min(lives, 10);
    float rightPanelX = 640 - rightPanelWidth;
    float iconSize = 20.f;
    float spacing = 1.f;
    for (int i = 0; i < lives; ++i) {
        sf::Sprite sprite;
        sprite.setTexture(heartTexture);
        sprite.setScale(iconSize / heartTexture.getSize().x, iconSize / heartTexture.getSize().y);
        int row = i / 5;
        int col = i % 5;
        sprite.setPosition(rightPanelX + 60 + col * (iconSize + spacing), 110 + row * (iconSize + spacing));
        lifeSprites.push_back(sprite);
    }
}

void GamePlayState::updateBombSprites() {
    bombSprites.clear();
    int bombs = player.getPowerUp();
    bombs = std::min(bombs, 4);
    float rightPanelX = 640 - rightPanelWidth;
    float iconSize = 20.f;
    float spacing = 1.f;
    for (int i = 0; i < bombs; ++i) {
        sf::Sprite sprite;
        sprite.setTexture(starTexture);
        sprite.setScale(iconSize / starTexture.getSize().x, iconSize / starTexture.getSize().y);
        sprite.setPosition(rightPanelX + 60 + i * (iconSize + spacing), 160);
        bombSprites.push_back(sprite);
    }
}
