#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu : public sf::Drawable {
public:
    Menu(float width, float height);
    void moveUp();
    void moveDown();
    int getSelectedItem() const { return selectedItemIndex; }
    void loadFont(const std::string& path);

private:
    int selectedItemIndex;
    sf::Font font;
    std::vector<sf::Text> menuItems;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif
