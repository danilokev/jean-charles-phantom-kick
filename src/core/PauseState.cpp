#include "../include/PauseState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"
#include "../include/MusicPlayer.h"
#include "../include/GamePlayState.h"
#include <iostream>

PauseState* PauseState::instance = nullptr;

PauseState::PauseState() : selectedOption(0) {
    // Cargar fuente
    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }
    
    // Configurar overlay gris semitransparente
    overlay.setSize(sf::Vector2f(640, 480));
    overlay.setFillColor(sf::Color(50, 50, 50, 150));
    
    // Configurar texto de pausa
    pauseText.setFont(font);
    pauseText.setString("PAUSA");
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setOutlineColor(sf::Color::Black);
    pauseText.setOutlineThickness(3.f);
    pauseText.setPosition(640/2 - pauseText.getLocalBounds().width/2, 100.f);
    
    // Configurar opción de continuar
    continueText.setFont(font);
    continueText.setString("Continuar");
    continueText.setCharacterSize(36);
    continueText.setFillColor(sf::Color::Red); // Opción seleccionada por defecto
    continueText.setOutlineColor(sf::Color::Black);
    continueText.setOutlineThickness(2.f);
    continueText.setPosition(640/2 - continueText.getLocalBounds().width/2, 200.f);
    
    // Configurar opción de salir
    exitText.setFont(font);
    exitText.setString("Salir al menu");
    exitText.setCharacterSize(36);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(2.f);
    exitText.setPosition(640/2 - exitText.getLocalBounds().width/2, 250.f);
}

PauseState* PauseState::getInstance() {
    if (!instance) instance = new PauseState();
    return instance;
}

void PauseState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down) {
                SoundPlayer::getInstance().playSound("menu_move");
                // Cambiar opción seleccionada
                if (selectedOption == 0) {
                    selectedOption = 1;
                    continueText.setFillColor(sf::Color::White);
                    exitText.setFillColor(sf::Color::Red);
                } else {
                    selectedOption = 0;
                    continueText.setFillColor(sf::Color::Red);
                    exitText.setFillColor(sf::Color::White);
                }
            }
            else if (event.key.code == sf::Keyboard::Z) {
                // Seleccionar opción
                if (selectedOption == 0) {
                    // Continuar - volver al juego
                    MusicPlayer::getInstance().resume();
                    manager.changeState(manager.getPreviousState());
                } else {
                    // Salir al menú principal
                    MusicPlayer::getInstance().stop();
                    GamePlayState::getInstance()->resetGame();
                    manager.changeState(MenuState::getInstance());
                }
            }
            else if (event.key.code == sf::Keyboard::X) {
                // Continuar - mismo comportamiento que seleccionar "Continuar"
                manager.changeState(manager.getPreviousState());
            }
        }
    }
}

void PauseState::update(sf::RenderWindow &, StateManager &) {
    // No hay updates específicos para este estado
}

void PauseState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    // Dibujar overlay
    renderManager->draw(overlay);
    
    // Dibujar textos
    renderManager->draw(pauseText);
    renderManager->draw(continueText);
    renderManager->draw(exitText);
}