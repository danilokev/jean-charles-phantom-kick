#include "../include/GameCompletedState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"
#include "../include/GamePlayState.h"
#include "../include/MusicPlayer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

GameCompletedState* GameCompletedState::instance = nullptr;

GameCompletedState::GameCompletedState() : currentScore(0) {
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
    
    // Configurar texto de Juego Completado
    gameCompletedText.setFont(font);
    gameCompletedText.setString("JUEGO COMPLETADO");
    gameCompletedText.setCharacterSize(48);
    gameCompletedText.setFillColor(sf::Color::Green);
    gameCompletedText.setOutlineColor(sf::Color::Black);
    gameCompletedText.setOutlineThickness(3.f);
    gameCompletedText.setPosition(640/2 - gameCompletedText.getLocalBounds().width/2, 100.f);
    
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
    exitText.setString("Presiona Z para volver al menu");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(640/2 - exitText.getLocalBounds().width/2, 400.f);
}

GameCompletedState* GameCompletedState::getInstance() {
    if (!instance) instance = new GameCompletedState();
    return instance;
}

void GameCompletedState::setScore(int score) {
    currentScore = score;
    scoreText.setString("Puntuacion: " + std::to_string(currentScore));
    scoreText.setPosition(640/2 - scoreText.getLocalBounds().width/2, 200.f);
}

void GameCompletedState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
            // Obtener la fecha actual en formato DD/MM/YYYY
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            std::ostringstream dateStream;
            dateStream << std::setfill('0') << std::setw(2) << localTime->tm_mday << "/"
                       << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "/"
                       << (localTime->tm_year + 1900);
            std::string date = dateStream.str();

            // Guardar la puntuación con la fecha como nombre
            std::ofstream file("resources/scores.txt", std::ios::app);
            if (file.is_open()) {
                file << date << " " << currentScore << "\n";
                file.close();
            } else {
                std::cerr << "No se pudo abrir el archivo de puntuaciones para escribir.\n";
            }

            MusicPlayer::getInstance().stop();
            GamePlayState::getInstance()->resetGame();
            manager.changeState(MenuState::getInstance());
        }
    }
}

void GameCompletedState::update(sf::RenderWindow &, StateManager &) {
    // No hay updates específicos para este estado
}

void GameCompletedState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    // Dibujar fondo
    renderManager->draw(backgroundSprite);
    
    // Dibujar textos
    renderManager->draw(gameCompletedText);
    renderManager->draw(scoreText);
    renderManager->draw(exitText);
}