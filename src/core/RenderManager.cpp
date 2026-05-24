#include "../include/RenderManager.h"

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager() : window(nullptr) {}

RenderManager* RenderManager::getInstance() {
    if (!instance) instance = new RenderManager();
    return instance;
}

void RenderManager::setWindow(sf::RenderWindow& win) {
    window = &win;
}

sf::RenderWindow* RenderManager::getWindow() const {
    return window;
}

void RenderManager::draw(const sf::Drawable& drawable) {
    if (window) window->draw(drawable);
}