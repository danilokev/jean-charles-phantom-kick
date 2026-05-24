#include "../include/StateManager.h"

StateManager::StateManager() : currentState(nullptr), previousState(nullptr), accumulator(0.0f) {}

void StateManager::changeState(GameState* state) {
    previousState = currentState;
    currentState = state;
}

void StateManager::handleInput(sf::RenderWindow &window) {
    if (currentState) currentState->handleInput(window, *this);
}

void StateManager::fixedUpdate(sf::RenderWindow &window) {
    if (currentState) currentState->update(window, *this);
}

void StateManager::renderWithInterpolation(sf::RenderWindow &window) {
    if (currentState) {
        currentState->render(window);
    }
}

void StateManager::update(sf::RenderWindow &window) {
    const float UPDATE_RATE = 1.0f / 60.0f;
    float deltaTime = updateClock.restart().asSeconds();
    accumulator += deltaTime;
    
    while (accumulator >= UPDATE_RATE) {
        fixedUpdate(window);
        accumulator -= UPDATE_RATE;
    }
}