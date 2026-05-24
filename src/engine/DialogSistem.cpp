// -*- coding: utf-8 -*-
#include "../include/DialogSistem.h"
#include <iostream>

DialogSistem::DialogSistem(unsigned int windowWidth, unsigned int windowHeight, const sf::Font& font)
: currentIndex(0), dialogueText("", font, 18), active(false), keyReleased(true)
{
    // Definir recuadro de texto
    float boxX = 40.f;
    float boxWidth = 360.f; // desde pixel 40 hasta 400
    float boxHeight = windowHeight / 4.f;
    float boxY = windowHeight - boxHeight - 20.f; // margen inferior de 20px

    box.setSize({boxWidth, boxHeight});
    box.setFillColor(sf::Color(30, 30, 30, 230));  // más opaco para mejor legibilidad
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color::White);
    box.setPosition(boxX, boxY);

    dialogueText.setFillColor(sf::Color::White);
    dialogueText.setCharacterSize(18);
}

void DialogSistem::addEntry(const std::string& imagePath, const std::string& text, bool alignRight) {
    Entry e;
    e.texture = std::make_shared<sf::Texture>();  // nueva textura por entrada
    if (!e.texture->loadFromFile(imagePath)) {
        std::cerr << "Error al cargar imagen: " << imagePath << std::endl;
    }
    e.portrait.setTexture(*e.texture); // usamos la textura desde el puntero
    
    // Escalar
    float scaleX = 250.f / e.texture->getSize().x;
    float scaleY = 250.f / e.texture->getSize().y;
    e.portrait.setScale(scaleX, scaleY);
    
    e.text = text;
    e.alignRight = alignRight;
    entries.push_back(e);  // ahora no se pierde la textura
}

void DialogSistem::start() {
    if (entries.empty()) return;
    currentIndex = 0;
    active = true;
    keyReleased = false;
}

void DialogSistem::update() {
    if (!active) return;
    bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    if (pressed && keyReleased) {
        currentIndex++;
        if (currentIndex >= entries.size()) {
            active = false;
            entries.clear();  // Limpia las entradas cuando termina el diálogo
        }
        keyReleased = false;
    } else if (!pressed) {
        keyReleased = true;
    }
}

bool DialogSistem::isActive() const {
    return active;
}

void DialogSistem::render(sf::RenderWindow& window) {
    if (!active || entries.empty()) return;
    Entry& e = entries[currentIndex];

    // Dibujar cuadro de texto
    window.draw(box);
    sf::Vector2f boxPos = box.getPosition();
    sf::Vector2f boxSize = box.getSize();

    // Obtener bounds de la imagen
    sf::FloatRect portraitBounds = e.portrait.getGlobalBounds();
    float imgX;
    // Ajustar posición de la imagen según alineación
    if (e.alignRight) {
        // Colocar a 10px del borde derecho de la ventana
        imgX = window.getSize().x - portraitBounds.width - 150.f;
    } else {
        // Colocar a 10px a la izquierda del recuadro
        imgX = boxPos.x - portraitBounds.width + 200.f;
    }
    float imgY = boxPos.y - portraitBounds.height - 10.f;
    e.portrait.setPosition(imgX, imgY);
    window.draw(e.portrait);

    // Posicionar texto dentro del recuadro con margen
    float textX = boxPos.x + 10.f;
    float textY = boxPos.y + 10.f;
    dialogueText.setPosition(textX, textY);
    dialogueText.setString(e.text);
    window.draw(dialogueText);
}