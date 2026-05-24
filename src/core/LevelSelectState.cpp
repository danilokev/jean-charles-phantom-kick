#include "../include/LevelSelectState.h"
#include "../include/GamePlayState.h"
#include "../include/MenuState.h"
#include "../include/WeaponSelectState.h"
#include "../include/RenderManager.h"
#include <iostream>

LevelSelectState* LevelSelectState::instance = nullptr;

LevelSelectState::LevelSelectState() : selectedLevelIndex(0), chosenLevel(-1) {
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
    
    std::vector<std::string> levelNames = {"Nivel 1", "Nivel 2", "Nivel 3", "Nivel 4", "Nivel 5"};
    for (size_t i = 0; i < levelNames.size(); i++) {
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setString(levelNames[i]);
        levelText.setCharacterSize(30);
        levelText.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        levelText.setOutlineColor(sf::Color::Black);
        levelText.setOutlineThickness(1.f);
        levelText.setPosition(640/2 - levelText.getLocalBounds().width/2, 150.f + i * 50.f);
        levelOptions.push_back(levelText);
    }
}

LevelSelectState* LevelSelectState::getInstance() {
    if (!instance) instance = new LevelSelectState();
    return instance;
}

void LevelSelectState::moveUp() {
    if (selectedLevelIndex > 0) {
        levelOptions[selectedLevelIndex].setFillColor(sf::Color::White);
        selectedLevelIndex--;
        levelOptions[selectedLevelIndex].setFillColor(sf::Color::Red);
    }
}

void LevelSelectState::moveDown() {
    if (selectedLevelIndex < levelOptions.size() - 1) {
        levelOptions[selectedLevelIndex].setFillColor(sf::Color::White);
        selectedLevelIndex++;
        levelOptions[selectedLevelIndex].setFillColor(sf::Color::Red);
    }
}

void LevelSelectState::handleInput(sf::RenderWindow &window, StateManager &manager) {
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
                chosenLevel = selectedLevelIndex;
                manager.changeState(WeaponSelectState::getInstance());
            }
            else if (event.key.code == sf::Keyboard::X) {
                manager.changeState(MenuState::getInstance());
            }
        }
    }
}

void LevelSelectState::update(sf::RenderWindow &, StateManager &) {}

void LevelSelectState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    renderManager->draw(backgroundSprite);
    
    for (const auto& levelText : levelOptions) {
        renderManager->draw(levelText);
    }
    
    renderManager->draw(instructionText);
}