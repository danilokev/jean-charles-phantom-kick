#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "GameState.h"
#include "StateManager.h"
#include "../include/Menu.h"
#include "../include/LevelSelectState.h"

class MenuState : public GameState {
private:
    Menu menu;
    static MenuState* instance;
    MenuState();
    
public:
    static MenuState* getInstance();
    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
};

#endif