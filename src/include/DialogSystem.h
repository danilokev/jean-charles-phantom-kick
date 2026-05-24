#ifndef DIALOGSYSTEM_H
#define DIALOGSYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Define algunos valores base para la ventana; ajústalos según sea necesario
const float WINDOW_WIDTH = 640.f;
const float WINDOW_HEIGHT = 480.f;

class DialogSystem {
public:
    // Constructor: recibe una lista de líneas de diálogo.
    DialogSystem(const std::vector<std::string>& dialogLines);

    // Inicia la secuencia de diálogos
    void start();

    // Actualiza el diálogo (animación de texto) usando el delta time dt
    void update(float dt);

    // Maneja la entrada (avanzar el diálogo) mediante la tecla Z
    void handleInput();

    // Dibuja la interfaz de diálogo (avatar, caja y texto)
    void draw(sf::RenderWindow& window);

    // Retorna true si el diálogo sigue activo, false si terminó
    bool isActive();

private:
    std::vector<std::string> dialogs;
    sf::Font font;
    sf::Text text;
    sf::RectangleShape textBox;
    sf::Texture avatarTexture;
    sf::Sprite avatarSprite;

    size_t currentDialogIndex;
    size_t charIndex;
    float elapsedTime;
    float textSpeed; // Tiempo (en segundos) por carácter, por ejemplo 0.05
    bool fullTextDisplayed;
    bool active;
    bool keyReleased;
};

#endif // DIALOGSYSTEM_H