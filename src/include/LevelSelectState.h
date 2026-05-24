#ifndef LEVELSELECTSTATE_H
#define LEVELSELECTSTATE_H

#include "GameState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class LevelSelectState : public GameState {
private:
    static LevelSelectState* instance;
    LevelSelectState();
    
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
    std::vector<sf::Text> levelOptions;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    size_t selectedLevelIndex = 0;
    int chosenLevel = -1; // Guardar el nivel para el modo practica

public:
    static LevelSelectState* getInstance();
    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
    
    void moveUp();
    void moveDown();
    int getSelectedLevel() const { return selectedLevelIndex; }
};

#endif