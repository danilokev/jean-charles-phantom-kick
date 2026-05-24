#include "../include/MenuState.h"
#include "../include/DialogState.h"
#include "../include/GamePlayState.h"
#include "../include/ScoreState.h"
#include "../include/WeaponSelectState.h"
#include "../include/LevelSelectState.h"
#include "../include/RenderManager.h"
#include "../include/MusicPlayer.h"
#include <SFML/Window.hpp>

MenuState* MenuState::instance = nullptr;

MenuState::MenuState() : menu(640, 480) {}

MenuState* MenuState::getInstance() {
    if (!instance) instance = new MenuState();
    return instance;
}

void MenuState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    if (!MusicPlayer::getInstance().isPlaying()) {
        MusicPlayer::getInstance().playTrack("menu_track");
    }
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                SoundPlayer::getInstance().playSound("menu_move");
                if (event.key.code == sf::Keyboard::Up)
                    menu.moveUp();
                else if (event.key.code == sf::Keyboard::Down)
                    menu.moveDown();
            }
            else if (event.key.code == sf::Keyboard::Z) {
                switch (menu.getSelectedItem()) {
                    case 0:  // Jugar
                        WeaponSelectState::getInstance()->setPracticeMode(false);
                        manager.changeState(WeaponSelectState::getInstance());
                        break;
                    case 1:  // Modo Practica
                        WeaponSelectState::getInstance()->setPracticeMode(true);
                        manager.changeState(LevelSelectState::getInstance());
                        break;
                    case 2:  // Puntuaciones
                        // Cambiar al estado de puntuaciones
                        manager.changeState(ScoreState::getInstance());
                        break;
                    case 3:  // Salir
                        window.close();
                        break;
                }
            }
                else if (event.key.code == sf::Keyboard::Num1) {
        manager.changeState(DialogState::getInstance());
    }
        }
    }
}

void MenuState::update(sf::RenderWindow &, StateManager &) {}

void MenuState::render(sf::RenderWindow &window) {
    if (MusicPlayer::getInstance().isPlaying() == false) {
        MusicPlayer::getInstance().playTrack("menu_track");
    }
    (void)window;
    RenderManager* renderManager = RenderManager::getInstance();
    renderManager->draw(menu);
}
