#ifndef WEAPONSELECTSTATE_H
#define WEAPONSELECTSTATE_H

#include "GameState.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class WeaponSelectState : public GameState {
private:
    static WeaponSelectState* instance;
    WeaponSelectState();
    
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
    std::vector<sf::Text> weaponOptions;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    size_t selectedWeaponIndex = 0;
    bool isPracticeMode = false;
    
public:
    static WeaponSelectState* getInstance();
    void handleInput(sf::RenderWindow &window, StateManager &manager) override;
    void update(sf::RenderWindow &window, StateManager &manager) override;
    void render(sf::RenderWindow &window) override;
    
    void moveUp();
    void moveDown();
    int getSelectedWeapon() const { return selectedWeaponIndex; }
    void setPracticeMode(bool practice) { isPracticeMode = practice; }
};

#endif