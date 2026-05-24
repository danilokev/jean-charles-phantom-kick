#ifndef SCREEN_FLASH_EFFECT_H
#define SCREEN_FLASH_EFFECT_H

#include <SFML/Graphics.hpp>

class ScreenFlashEffect {
public:
    // Constructor
    ScreenFlashEffect();

    // Inicia el efecto, reiniciando el tiempo y activándolo
    void start();

    // Actualiza el efecto según el delta time (dt)
    void update(float dt);

    // Devuelve el overlay para dibujarlo
    const sf::RectangleShape& getOverlay() const;

    // Indica si el efecto sigue activo
    bool isActive() const;

private:
    sf::RectangleShape overlay; // Capa de color que se dibuja encima de la pantalla
    float lifetime;             // Duración total del efecto en segundos
    float elapsed;              // Tiempo transcurrido desde que se inició el efecto
    bool active;                // Indica si el efecto está activo
};

#endif // SCREEN_FLASH_EFFECT_H
