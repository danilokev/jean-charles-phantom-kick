#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "GameState.h"
#include <SFML/Graphics.hpp>

class PauseState : public GameState {
public:
    static PauseState* getInstance();
    
    void handleInput(sf::RenderWindow& window, StateManager& manager) override;
    void update(sf::RenderWindow& window, StateManager& manager) override;
    void render(sf::RenderWindow& window) override;
    
private:
    PauseState();
    static PauseState* instance;
    
    sf::Font font;
    sf::RectangleShape overlay;
    sf::Text pauseText;
    sf::Text continueText;
    sf::Text exitText;
    int selectedOption;
};

#endif