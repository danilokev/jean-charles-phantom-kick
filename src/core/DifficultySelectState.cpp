#include "../include/DifficultySelectState.h"
#include "../include/GamePlayState.h"
#include "../include/WeaponSelectState.h"
#include "../include/RenderManager.h"
#include "../include/MusicPlayer.h"
#include <iostream>

DifficultySelectState* DifficultySelectState::instance = nullptr;

DifficultySelectState::DifficultySelectState() : selectedDifficultyIndex(0) {
    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }
    
    if (!backgroundTexture.loadFromFile("resources/select_W.png")) {
        std::cerr << "Error cargando la imagen de fondo.\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    
    instructionText.setFont(font);
    instructionText.setString("Presiona Z para seleccionar, X para volver");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(1.f);
    instructionText.setPosition(640/2 - instructionText.getLocalBounds().width/2, 420.f);
    
    std::vector<std::string> difficultyNames = {"Facil", "Normal", "Dificil"};
    for (size_t i = 0; i < difficultyNames.size(); i++) {
        sf::Text difficultyText;
        difficultyText.setFont(font);
        difficultyText.setString(difficultyNames[i]);
        difficultyText.setCharacterSize(30);
        difficultyText.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        difficultyText.setOutlineColor(sf::Color::Black);
        difficultyText.setOutlineThickness(1.f);
        difficultyText.setPosition(640/2 - difficultyText.getLocalBounds().width/2, 150.f + i * 50.f);
        difficultyOptions.push_back(difficultyText);
    }
}

DifficultySelectState* DifficultySelectState::getInstance() {
    if (!instance) instance = new DifficultySelectState();
    return instance;
}

void DifficultySelectState::moveUp() {
    if (selectedDifficultyIndex > 0) { 
        difficultyOptions[selectedDifficultyIndex].setFillColor(sf::Color::White);
        selectedDifficultyIndex--;
        difficultyOptions[selectedDifficultyIndex].setFillColor(sf::Color::Red);
    }
}

void DifficultySelectState::moveDown() {
    if (selectedDifficultyIndex < difficultyOptions.size() - 1) {
        difficultyOptions[selectedDifficultyIndex].setFillColor(sf::Color::White);
        selectedDifficultyIndex++;
        difficultyOptions[selectedDifficultyIndex].setFillColor(sf::Color::Red);
    }
}

void DifficultySelectState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                SoundPlayer::getInstance().playSound("menu_move");
                if (event.key.code == sf::Keyboard::Up)
                    moveUp();
                else if (event.key.code == sf::Keyboard::Down)
                    moveDown();
            }
            else if (event.key.code == sf::Keyboard::Z) {
                GamePlayState* gamePlayState = GamePlayState::getInstance();
                switch (selectedDifficultyIndex) {
                    case 0: // Fácil
                        gamePlayState->setPlayerLivesAndBombs(10, 3);
                        break;
                    case 1: // Normal
                        gamePlayState->setPlayerLivesAndBombs(6, 2);
                        break;
                    case 2: // Difícil
                        gamePlayState->setPlayerLivesAndBombs(4, 1);
                        break;
                }
                MusicPlayer::getInstance().stop();
                MusicPlayer::getInstance().playTrack("level1_track");
                manager.changeState(gamePlayState);
            }
            else if (event.key.code == sf::Keyboard::X) {
                manager.changeState(WeaponSelectState::getInstance());
            }
        }
    }
}

void DifficultySelectState::update(sf::RenderWindow &, StateManager &) {}

void DifficultySelectState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    renderManager->draw(backgroundSprite);
    
    for (const auto& difficultyText : difficultyOptions) {
        renderManager->draw(difficultyText);
    }
    
    renderManager->draw(instructionText);
}