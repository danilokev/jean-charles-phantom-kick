#ifndef GAMECOMPLETEDSTATE_H
#define GAMECOMPLETEDSTATE_H

#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <string>

class GameCompletedState : public GameState {
private:
    GameCompletedState();
    static GameCompletedState* instance;
    
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text gameCompletedText;
    sf::Text scoreText;
    sf::Text exitText;
    int currentScore;


public:
    static GameCompletedState* getInstance();
    void handleInput(sf::RenderWindow& window, StateManager& manager) override;
    void update(sf::RenderWindow& window, StateManager& manager) override;
    void render(sf::RenderWindow& window) override;
    void setScore(int score);  
};

#endif