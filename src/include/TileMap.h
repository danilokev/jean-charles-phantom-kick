#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include <vector>
#include <string>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    TileMap();
    bool load(const std::string& tmxFile);
    void update(float deltaTime);
    void updateView(const sf::View& view);
    void reset();
    

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture _tilesetTexture;
    sf::VertexArray _vertices;
    std::vector<std::vector<std::vector<int>>> _tilemap;
    int _tileWidth;
    int _tileHeight;
    int _mapWidth;
    int _mapHeight;
    int _layers;

    float _scrollSpeed;
    float _currentOffset;

    void parseTMX(const std::string& tmxFile);
    void loadTileset(const std::string& tilesetFile);
    void buildVertexArray();
    void scrollMap();
    void regenerateTiles();
};

#endif // TILEMAP_H
