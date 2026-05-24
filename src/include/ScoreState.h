#ifndef SCORESTATE_H
#define SCORESTATE_H

#include "GameState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ScoreState : public GameState {
private:
    static ScoreState* instance;
    ScoreState();
    
    sf::Font font;
    sf::Text titleText;
    sf::Text exitText;
    std::vector<sf::Text> scoreTexts;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<std::pair<std::string, int>> scores;
    
    void loadScores(const std::string& path);
    
public:
    static ScoreState* getInstance();
    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
};

#endif