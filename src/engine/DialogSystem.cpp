#include "../include/DialogState.h"
#include <iostream>

DialogSystem::DialogSystem(const std::vector<std::string>& dialogLines)
    : dialogs(dialogLines),
      currentDialogIndex(0),
      charIndex(0),
      elapsedTime(0.f),
      textSpeed(0.05f),
      fullTextDisplayed(false),
      active(false),
      keyReleased(true)
{
    // Cargar la fuente
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
    }

    // Configurar la caja de diálogo (puedes ajustar el tamaño y la posición)
    textBox.setSize(sf::Vector2f(WINDOW_WIDTH * 0.8f, 100.f));
    textBox.setFillColor(sf::Color(0, 0, 0, 150));
    textBox.setPosition(WINDOW_WIDTH * 0.1f, WINDOW_HEIGHT * 0.75f);

    // Configurar el texto
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(WINDOW_WIDTH * 0.12f, WINDOW_HEIGHT * 0.78f);

    // Cargar la imagen del avatar
    if (!avatarTexture.loadFromFile("resources/d11.png")) {
        std::cerr << "Error al cargar la imagen del avatar" << std::endl;
    }
    avatarSprite.setTexture(avatarTexture);
    // Puedes ajustar la escala u origen aquí o en draw() si lo deseas.
}

void DialogSystem::start() {
    active = true;
    currentDialogIndex = 0;
    charIndex = 0;
    fullTextDisplayed = false;
    text.setString("");
    elapsedTime = 0.f;
}

void DialogSystem::update(float dt) {
    if (!active) return;
    
    if (!fullTextDisplayed) {
        elapsedTime += dt;
        if (elapsedTime >= textSpeed && charIndex < dialogs[currentDialogIndex].size()) {
            text.setString(dialogs[currentDialogIndex].substr(0, charIndex + 1));
            charIndex++;
            elapsedTime = 0.f;
        }
        if (charIndex >= dialogs[currentDialogIndex].size()) {
            fullTextDisplayed = true;
        }
    }
}

void DialogSystem::handleInput() {
    if (!active) return;

    // Se usa la tecla Z para avanzar en el diálogo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && keyReleased) {
        keyReleased = false;
        if (!fullTextDisplayed) {
            // Mostrar texto completo
            text.setString(dialogs[currentDialogIndex]);
            charIndex = dialogs[currentDialogIndex].size();
            fullTextDisplayed = true;
        } else {
            // Avanzar al siguiente diálogo
            currentDialogIndex++;
            if (currentDialogIndex < dialogs.size()) {
                charIndex = 0;
                fullTextDisplayed = false;
                text.setString("");
            } else {
                // Fin del diálogo
                active = false;
            }
        }
    }
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        keyReleased = true;
    }
}

void DialogSystem::draw(sf::RenderWindow& window) {
    if (active) {
        // Seleccionar la porción del avatar según el diálogo:
        // Si el índice (1-based) es impar, se usa la mitad izquierda; si es par, la derecha.
        if ((currentDialogIndex + 1) % 2 == 1) {
            avatarSprite.setTextureRect(sf::IntRect(0, 0, 250, 500));
        } else {
            avatarSprite.setTextureRect(sf::IntRect(250, 0, 250, 500));
        }
        
        // Ajustar la escala del avatar (por ejemplo, escalado a la mitad)
        avatarSprite.setScale(0.5f, 0.5f);

        float margin = 10.f;
        float textBoxY = textBox.getPosition().y;
        float textBoxHeight = textBox.getSize().y;
        float avatarHeight = avatarSprite.getGlobalBounds().height;
        float avatarY = textBoxY + (textBoxHeight - avatarHeight) / 2.f;

        if ((currentDialogIndex + 1) % 2 == 1) {
            // Avatar a la izquierda del textBox
            float avatarX = textBox.getPosition().x - avatarSprite.getGlobalBounds().width - margin;
            avatarSprite.setPosition(avatarX, avatarY);
        } else {
            // Avatar a la derecha del textBox
            float avatarX = textBox.getPosition().x + textBox.getSize().x + margin;
            avatarSprite.setPosition(avatarX, avatarY);
        }
        
        window.draw(avatarSprite);
        window.draw(textBox);
        window.draw(text);
    }
}

bool DialogSystem::isActive() {
    return active;
}