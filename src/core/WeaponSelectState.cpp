#include "../include/WeaponSelectState.h"
#include "../include/DifficultySelectState.h"
#include "../include/GamePlayState.h"
#include "../include/LevelSelectState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"
#include <iostream>

WeaponSelectState* WeaponSelectState::instance = nullptr;

WeaponSelectState::WeaponSelectState() : selectedWeaponIndex(0), isPracticeMode(true) {
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
    
    std::vector<std::string> weaponNames = {"Disparo estandar", "Disparo de area", "Disparo disperso", "Disparo de fuerza"};
    for (size_t i = 0; i < weaponNames.size(); i++) {
        sf::Text weaponText;
        weaponText.setFont(font);
        weaponText.setString(weaponNames[i]);
        weaponText.setCharacterSize(30);
        weaponText.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        weaponText.setOutlineColor(sf::Color::Black);
        weaponText.setOutlineThickness(1.f);
        weaponText.setPosition(640/2 - weaponText.getLocalBounds().width/2, 150.f + i * 50.f);
        weaponOptions.push_back(weaponText);
    }
}

WeaponSelectState* WeaponSelectState::getInstance() {
    if (!instance) instance = new WeaponSelectState();
    return instance;
}

void WeaponSelectState::moveUp() {
    if (selectedWeaponIndex > 0) {
        weaponOptions[selectedWeaponIndex].setFillColor(sf::Color::White);
        selectedWeaponIndex--;
        weaponOptions[selectedWeaponIndex].setFillColor(sf::Color::Red);
    }
}

void WeaponSelectState::moveDown() {
    if (selectedWeaponIndex < weaponOptions.size() - 1) {
        weaponOptions[selectedWeaponIndex].setFillColor(sf::Color::White);
        selectedWeaponIndex++;
        weaponOptions[selectedWeaponIndex].setFillColor(sf::Color::Red);
    }
}

void WeaponSelectState::handleInput(sf::RenderWindow &window, StateManager &manager) {
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
                GamePlayState::getInstance()->setSelectedWeapon(selectedWeaponIndex);
                if (isPracticeMode) {
                    int selectedLevel = LevelSelectState::getInstance()->getSelectedLevel();
                    GamePlayState::getInstance()->resetGame();
                    GamePlayState::getInstance()->setPlayerLivesAndBombs(99, 3);
                    GamePlayState::getInstance()->practiceModeStart(selectedLevel);
                    isPracticeMode = false; // Reset practice mode flag
                    manager.changeState(GamePlayState::getInstance());
                } else {
                    manager.changeState(DifficultySelectState::getInstance());
                }
            }
            else if (event.key.code == sf::Keyboard::X) {
                manager.changeState(MenuState::getInstance());
            }
        }
    }
}

void WeaponSelectState::update(sf::RenderWindow &, StateManager &) {}

void WeaponSelectState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    renderManager->draw(backgroundSprite);
    
    for (const auto& weaponText : weaponOptions) {
        renderManager->draw(weaponText);
    }
    
    renderManager->draw(instructionText);
}