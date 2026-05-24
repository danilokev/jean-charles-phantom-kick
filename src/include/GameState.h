#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

class StateManager;

class GameState {
public:
    virtual ~GameState() {}
    virtual void handleInput(sf::RenderWindow &window, StateManager &manager) = 0;
    virtual void update(sf::RenderWindow &window, StateManager &manager) = 0;
    virtual void render(sf::RenderWindow &window) = 0;
};

#endif