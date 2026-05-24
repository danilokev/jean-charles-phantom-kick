// -*- coding: utf-8 -*-
#ifndef DIALOGSISTEM_H
#define DIALOGSISTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory> // <-- necesario para shared_ptr

class DialogSistem {
public:
    struct Entry {
        sf::Sprite portrait;
        std::shared_ptr<sf::Texture> texture;  // CAMBIO AQUÍ
        std::string text;
        bool alignRight;  // si true, imagen a la derecha, texto a la izquierda
    };

    DialogSistem(unsigned int windowWidth, unsigned int windowHeight, const sf::Font& font);

    // Agrega una nueva entrada de diálogo con imagen, texto y alineación
    void addEntry(const std::string& imagePath, const std::string& text, bool alignRight = false);
    // Inicia el diálogo
    void start();
    // Dibuja el cuadro de diálogo y la imagen actual
    void render(sf::RenderWindow& window);
    // Maneja input y avanza paso a paso
    void update();
    // Detecta si el diálogo está activo
    bool isActive() const;

private:
    std::vector<Entry> entries;
    std::size_t currentIndex;
    sf::RectangleShape box;
    sf::Text dialogueText;
    sf::Texture portraitTexture;
    bool active;
    bool keyReleased; // control de paso por pulsación
};

#endif // DIALOGSISTEM_H