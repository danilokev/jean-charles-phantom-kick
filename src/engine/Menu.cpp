#include "../include/Menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Menu::Menu(float width, float height) {
    // Cargar fuente estilo Touhou (debes tener el archivo en resources/)
    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }

    if (!backgroundTexture.loadFromFile("resources/fondo2.png")) {
        std::cerr << "Error cargando la imagen de fondo.\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    std::vector<std::string> items = { "Jugar", "Modo Practica", "Puntuaciones", "Salir" };
    for (size_t i = 0; i < items.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(items[i]);
        text.setCharacterSize(36);  // Tamaño más grande para estilo Touhou
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(2.f);
        text.setPosition(sf::Vector2f(width / 2 - text.getLocalBounds().width/2, 
                                     height / (items.size() + 1) * (i + 1)));
        menuItems.push_back(text);
    }

    selectedItemIndex = 0;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(backgroundSprite, states);
    
    // Dibujar opciones del menú
    for (const auto& item : menuItems) {
        target.draw(item, states);
    }
}

void Menu::moveUp() {
    if (menuItems.empty()) return;

    if (selectedItemIndex > 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (menuItems.empty()) return;

    if (selectedItemIndex < static_cast<int>(menuItems.size()) - 1) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        ++selectedItemIndex;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
