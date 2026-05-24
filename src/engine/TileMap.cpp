#include "../include/TileMap.h"
#include <iostream>

TileMap::TileMap() 
    : _tileWidth(0), _tileHeight(0), _mapWidth(0), _mapHeight(0), 
      _layers(0), _scrollSpeed(50.0f), _currentOffset(0.0f) {}

bool TileMap::load(const std::string& tmxFile) {
    reset();
    parseTMX(tmxFile);
    buildVertexArray();
    return true;
}

void TileMap::reset() {
    _tilemap.clear();
    _vertices.clear();
    _tilesetTexture.~Texture(); // destruye explícitamente la textura
    new (&_tilesetTexture) sf::Texture(); // la vuelve a construir in-place    _layers = 0;
    _currentOffset = 0.0f;
    _mapWidth = 0;
    _mapHeight = 0;
    _tileWidth = 0;
    _tileHeight = 0;
    _vertices.clear();         // limpia los vértices
    _vertices.resize(0);       // garantiza que no queden vértices asignados
    _vertices.setPrimitiveType(sf::Quads); // por si luego se vuelve a construir

    // Opcional: resetear la posición si el mapa fue transformado
    setPosition(0.f, 0.f);
    setRotation(0.f);
    setScale(1.f, 1.f);
}


std::string resolvePath(const std::string& basePath, const std::string& relativePath) {
    size_t lastSlash = basePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return basePath.substr(0, lastSlash + 1) + relativePath;
    }
    return relativePath;
}

void TileMap::parseTMX(const std::string& tmxFile) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(tmxFile.c_str()) != tinyxml2::XML_SUCCESS) {
        return;
    }

    auto mapNode = doc.FirstChildElement("map");
    if (!mapNode) {
        std::cerr << "[ERROR] No se encontró el elemento <map> en el TMX." << std::endl;
        return;
    }

    _mapWidth = mapNode->IntAttribute("width");
    _mapHeight = mapNode->IntAttribute("height");
    _tileWidth = mapNode->IntAttribute("tilewidth");
    _tileHeight = mapNode->IntAttribute("tileheight");

    // Carga del tileset
    auto tilesetNode = mapNode->FirstChildElement("tileset");
    if (!tilesetNode) {
        std::cerr << "[ERROR] No se encontró el elemento <tileset> en el TMX." << std::endl;
        return;
    }

    std::string source = tilesetNode->Attribute("source");
    std::string fullPath = resolvePath(tmxFile, source);
    loadTileset(fullPath);

    //Nueva inicialización del vector 3D
    _tilemap.clear(); // Limpiamos por si acaso
    _layers = 0;

    // Recorremos las capas
    for (auto layerNode = mapNode->FirstChildElement("layer"); layerNode; layerNode = layerNode->NextSiblingElement("layer")) {
        int width = layerNode->IntAttribute("width");
        int height = layerNode->IntAttribute("height");

        std::vector<std::vector<int>> layerData(height, std::vector<int>(width, 0));
        auto dataNode = layerNode->FirstChildElement("data");
        
        if (!dataNode) {
            std::cerr << "[ERROR] No se encontró <data> en la capa: " << layerNode->Attribute("name") << std::endl;
            continue;
        }

        auto tileNode = dataNode->FirstChildElement("tile");

        if (!tileNode) {
            std::cerr << "[ERROR] No se encontraron tiles en <data>." << std::endl;
            continue;
        }

        int y = 0, x = 0;
        while (tileNode) {
            int gid = tileNode->IntAttribute("gid");
            layerData[y][x] = gid;
            
            if (++x >= width) {
                x = 0;
                ++y;
            }
            tileNode = tileNode->NextSiblingElement("tile");
        }

        _tilemap.push_back(layerData);
        _layers++;
    }
}

void TileMap::loadTileset(const std::string& tilesetFile) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(tilesetFile.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading Tileset XML: " << tilesetFile << " | Error: " 
                  << doc.ErrorStr() << std::endl;
        return;
    }

    // Buscamos el nodo <image> que contiene la ruta de la textura
    auto imageNode = doc.FirstChildElement("tileset")->FirstChildElement("image");
    if (!imageNode) {
        std::cerr << "No se encontró la imagen dentro del tileset: " << tilesetFile << std::endl;
        return;
    }

    // Obtenemos el path de la imagen
    std::string imagePath = imageNode->Attribute("source");
    
    // Resolver la ruta completa
    std::string fullPath = resolvePath(tilesetFile, imagePath);

    std::cout << "Cargando textura desde: " << fullPath << std::endl;

    // Cargamos la textura
    if (!_tilesetTexture.loadFromFile(fullPath)) {
        std::cerr << "Error loading tileset image: " << fullPath << std::endl;
    }
}
void TileMap::buildVertexArray() {
    _vertices.setPrimitiveType(sf::Quads);
    _vertices.resize(_mapWidth * _mapHeight * 4 * _layers);

    int index = 0;
    for (int layer = 0; layer < _layers; ++layer) {
        for (int y = 0; y < _mapHeight; ++y) {
            for (int x = 0; x < _mapWidth; ++x) {
                int gid = _tilemap[layer][y][x];
                if (gid == 0) continue;

                int tu = (gid - 1) % (_tilesetTexture.getSize().x / _tileWidth);
                int tv = (gid - 1) / (_tilesetTexture.getSize().x / _tileWidth);

                sf::Vertex* quad = &_vertices[index * 4];

                quad[0].position = sf::Vector2f(x * _tileWidth, y * _tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * _tileWidth, y * _tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * _tileWidth, (y + 1) * _tileHeight);
                quad[3].position = sf::Vector2f(x * _tileWidth, (y + 1) * _tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * _tileWidth, tv * _tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * _tileWidth, tv * _tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * _tileWidth, (tv + 1) * _tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * _tileWidth, (tv + 1) * _tileHeight);

                index++;
            }
        }
    }
}

void TileMap::update(float deltaTime) {
    // Invertimos el sentido del scroll (ahora hacia abajo)
    _currentOffset -= _scrollSpeed * deltaTime;

    // Si llegamos al final del mapa visible (parte superior), regeneramos
    if (_currentOffset-_tileHeight <= -_tileHeight) {
        _currentOffset += _tileHeight;
        regenerateTiles();
    }

    // Aplicamos el desplazamiento a la posición del mapa
    scrollMap();
}

void TileMap::regenerateTiles() {
    for (int layer = 0; layer < _layers; ++layer) {
        // Guardamos la última fila
        auto lastRow = _tilemap[layer][_mapHeight - 1];

        // Movemos todas las filas una posición hacia abajo
        for (int y = _mapHeight - 1; y > 0; --y) {
            _tilemap[layer][y] = _tilemap[layer][y - 1];
        }

        // Insertamos la última fila en la primera posición
        _tilemap[layer][0] = lastRow;
    }

    // Volvemos a construir el VertexArray para reflejar los cambios
    buildVertexArray();
}

void TileMap::scrollMap() {
    // Aplicamos el offset y los márgenes iniciales (32, 16)
    setPosition(32, 16 - _currentOffset);
}

void TileMap::updateView(const sf::View& view) {
    setPosition(view.getCenter().x - (view.getSize().x / 2), 
                view.getCenter().y - (view.getSize().y / 2));
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &_tilesetTexture;
    target.draw(_vertices, states);
}
