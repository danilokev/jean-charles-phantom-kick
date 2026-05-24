#ifndef DIFFICULTYSELECTSTATE_H
#define DIFFICULTYSELECTSTATE_H

#include "GameState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DifficultySelectState : public GameState {
private:
    static DifficultySelectState* instance;
    DifficultySelectState();
    
    sf::Font font;
    sf::Text instructionText;
    std::vector<sf::Text> difficultyOptions;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    size_t selectedDifficultyIndex = 0;
    
public:
    static DifficultySelectState* getInstance();
    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
    
    void moveUp();
    void moveDown();
    int getSelectedDifficulty() const { return selectedDifficultyIndex; }
};

#endif