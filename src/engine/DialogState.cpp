#include "../include/DialogState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"

#include <SFML/Window.hpp>
#include <iostream>

// Inicializamos el singleton a nullptr
DialogState* DialogState::instance = nullptr;

// Puedes definir una lista de diálogos de ejemplo
static std::vector<std::string> ejemploDialogos = {
    "Hola, bienvenido a mi juego.",
    "Esta es una prueba de dialogos.",
    "Presiona Z para avanzar.",
    "Fin de los dialogos."
};

DialogState::DialogState(const std::vector<std::string>& dialogLines)
    : dialog(dialogLines)
{
    dialog.start();
}

DialogState* DialogState::getInstance() {
    if (instance == nullptr)
        instance = new DialogState(ejemploDialogos);
    return instance;
}

void DialogState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    // Delegamos la entrada al DialogSystem
    dialog.handleInput();

    // Opcional: si se presiona X, se puede cancelar el diálogo y volver al menú
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
            // Cancelamos el diálogo y volvemos al menú
            manager.changeState(MenuState::getInstance());
        }
    }
}

void DialogState::update(sf::RenderWindow &, StateManager &manager) {
    dialog.update(1.0f / 60.0f);

    if (!dialog.isActive()) {
        manager.changeState(MenuState::getInstance());
    }
}


void DialogState::render(sf::RenderWindow &window) {
    window.clear(sf::Color::Black); // Fondo negro o el que prefieras
    // Dibujamos el sistema de diálogos
    dialog.draw(window);
    window.display();
}