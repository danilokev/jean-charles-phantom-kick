#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "GameState.h"
#include <memory>
#include <SFML/System.hpp>

class StateManager {
private:
    GameState* currentState;
    GameState* previousState;
    float accumulator;
    sf::Clock updateClock;
    
public:
    StateManager();
    void changeState(GameState* state);
    void handleInput(sf::RenderWindow &window);
    void fixedUpdate(sf::RenderWindow &window);
    void renderWithInterpolation(sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
    GameState* getPreviousState() const { return previousState; }
};

#endif