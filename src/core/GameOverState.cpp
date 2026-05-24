#include "../include/GameOverState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"
#include "../include/GamePlayState.h"
#include "../include/MusicPlayer.h"
#include <iostream>

GameOverState* GameOverState::instance = nullptr;

GameOverState::GameOverState() : currentScore(0) {
    // Cargar fuente
    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }
    
    // Cargar fondo (el mismo que ScoreState)
    if (!backgroundTexture.loadFromFile("resources/Scores_R.png")) {
        std::cerr << "Error cargando la imagen de fondo.\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    
    // Configurar texto de Game Over
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(3.f);
    gameOverText.setPosition(640/2 - gameOverText.getLocalBounds().width/2, 100.f);
    
    // Configurar texto de puntuación
    scoreText.setFont(font);
    scoreText.setString("Puntuacion: 0");
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.f);
    scoreText.setPosition(640/2 - scoreText.getLocalBounds().width/2, 200.f);
    
    // Configurar texto de salida
    exitText.setFont(font);
    exitText.setString("Presiona Z para volver al menu y continuar, presiona X para reiniciar");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(640/2 - exitText.getLocalBounds().width/2, 400.f);
}

GameOverState* GameOverState::getInstance() {
    if (!instance) instance = new GameOverState();
    return instance;
}

void GameOverState::setScore(int score) {
    currentScore = score;
    scoreText.setString("Puntuacion: " + std::to_string(currentScore));
    scoreText.setPosition(640/2 - scoreText.getLocalBounds().width/2, 200.f);
}

void GameOverState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
            MusicPlayer::getInstance().stop();
            manager.changeState(MenuState::getInstance());
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
            MusicPlayer::getInstance().stop();
            manager.changeState(MenuState::getInstance());
            GamePlayState::getInstance()->resetGame();
        }
    }
}

void GameOverState::update(sf::RenderWindow &, StateManager &) {
    // No hay updates específicos para este estado
}

void GameOverState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    // Dibujar fondo
    renderManager->draw(backgroundSprite);
    
    // Dibujar textos
    renderManager->draw(gameOverText);
    renderManager->draw(scoreText);
    renderManager->draw(exitText);
}