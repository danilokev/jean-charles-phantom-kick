#include "../include/ScreenFlashEffect.h"
#include <iostream>

ScreenFlashEffect::ScreenFlashEffect()
    : lifetime(1.5f), elapsed(0.0f), active(false)
{
    // Se supone que la ventana es de 1280x960; ajusta el tamaño si es necesario
    overlay.setSize(sf::Vector2f(1280.f, 960.f));
    overlay.setFillColor(sf::Color(255, 0, 0, 255));
}

void ScreenFlashEffect::start() {
    active = true;
    elapsed = 0.0f;
    // Reiniciamos el overlay al color rojo opaco
    overlay.setFillColor(sf::Color(255, 0, 0, 255));
}

void ScreenFlashEffect::update(float dt) {
    if (!active) return;

    elapsed += dt;
    float progress = elapsed / lifetime;
    int alpha = static_cast<int>(255 * (1 - progress));

    // Se actualiza la transparencia del overlay
    sf::Color currentColor = overlay.getFillColor();
    currentColor.a = alpha;
    overlay.setFillColor(currentColor);

    if (progress >= 1.0f) {
        active = false;
    }
}

const sf::RectangleShape& ScreenFlashEffect::getOverlay() const {
    return overlay;
}

bool ScreenFlashEffect::isActive() const {
    return active;
}
