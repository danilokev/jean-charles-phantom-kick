#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <SFML/Graphics.hpp>
#include <memory>

class RenderManager {
private:
    static RenderManager* instance;
    sf::RenderWindow* window;
    RenderManager();
    
public:
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    
    static RenderManager* getInstance();
    void setWindow(sf::RenderWindow& win);
    sf::RenderWindow* getWindow() const;
    void draw(const sf::Drawable& drawable);
};

#endif