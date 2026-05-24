#include "../include/ScoreState.h"
#include "../include/MenuState.h"
#include "../include/RenderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Inicialización del singleton de ScoreState
ScoreState* ScoreState::instance = nullptr;

ScoreState::ScoreState() {
    // Cargar fuente
    if (!font.loadFromFile("resources/thfont.ttf")) {
        std::cerr << "Error cargando la fuente Touhou. Usando Arial por defecto.\n";
        if (!font.loadFromFile("resources/arial.ttf")) {
            std::cerr << "Error cargando la fuente por defecto.\n";
        }
    }
    
    // Cargar fondo
    if (!backgroundTexture.loadFromFile("resources/Scores_R.png")) {
        std::cerr << "Error cargando la imagen de fondo.\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    
    // Configurar título
    titleText.setFont(font);
    titleText.setString("TOP PUNTUACIONES");
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    titleText.setPosition(640/2 - titleText.getLocalBounds().width/2, 50.f);
    
    // Configurar texto de salida
    exitText.setFont(font);
    exitText.setString("Presiona X para volver al menu");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(640/2 - exitText.getLocalBounds().width/2, 420.f);
    
    // Cargar las puntuaciones
    loadScores("resources/scores.txt");
}

void ScoreState::loadScores(const std::string& path) {
    scores.clear();
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (iss >> name >> score) {
                scores.emplace_back(name, score);
            }
        }
        file.close();
        
        // Ordenar puntuaciones de mayor a menor
        std::sort(scores.begin(), scores.end(), 
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        // Limitar a 10 puntuaciones
        if (scores.size() > 10) {
            scores.resize(10);
        }
        
        // Guardar las puntuaciones ordenadas de vuelta al archivo
        std::ofstream outFile(path);
        if (outFile.is_open()) {
            for (const auto& score : scores) {
                outFile << score.first << " " << score.second << "\n";
            }
            outFile.close();
        } else {
            std::cerr << "No se pudo abrir el archivo de puntuaciones para escribir.\n";
        }
        
        // Crear los textos para las puntuaciones
        scoreTexts.clear();
        for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(10)); i++) {
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setString(std::to_string(i+1) + ". " + scores[i].first + " - " + 
                               std::to_string(scores[i].second));
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setOutlineColor(sf::Color::Black);
            scoreText.setOutlineThickness(1.f);
            scoreText.setPosition(640/2 - scoreText.getLocalBounds().width/2, 120.f + i * 30.f);
            scoreTexts.push_back(scoreText);
        }
    } else {
        std::cerr << "No se pudo abrir el archivo de puntuaciones.\n";
    }
}

ScoreState* ScoreState::getInstance() {
    if (!instance) instance = new ScoreState();
    return instance;
}

void ScoreState::handleInput(sf::RenderWindow &window, StateManager &manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X) {
            manager.changeState(MenuState::getInstance());
        }
    }
}

void ScoreState::update(sf::RenderWindow &, StateManager &) {
    // No hay updates específicos para este estado
}

void ScoreState::render(sf::RenderWindow &) {
    RenderManager* renderManager = RenderManager::getInstance();
    
    // Dibujar fondo
    renderManager->draw(backgroundSprite);
    
    // Dibujar título
    renderManager->draw(titleText);
    
    // Dibujar puntuaciones
    for (const auto& scoreText : scoreTexts) {
        renderManager->draw(scoreText);
    }
    
    // Dibujar texto de salida
    renderManager->draw(exitText);
}
