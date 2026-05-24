#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/Menu.h"
#include "core/StateManager.cpp"
#include "include/MenuState.h"
#include "include/GameOverState.h"
#include "include/PauseState.h"
#include "include/ScoreState.h"
#include "include/WeaponSelectState.h"
#include "include/DifficultySelectState.h"
#include "include/RenderManager.h"
#include "include/BoundingCircle.h"
#include "include/DirectionalMovement.h"
#include "include/SoundPlayer.h"
#include "include/MusicPlayer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Juego");
    RenderManager* renderManager = RenderManager::getInstance();
    renderManager->setWindow(window);

    SoundPlayer::loadAllSounds();

    StateManager stateManager;
    stateManager.changeState(MenuState::getInstance());

    MusicPlayer& musicPlayer = MusicPlayer::getInstance();
    musicPlayer.loadTrack("menu_track",   "resources/music/pista01.ogg");
    musicPlayer.loadTrack("level1_track", "resources/music/pista02.ogg");
    musicPlayer.loadTrack("level2_track", "resources/music/pista10.ogg");
    musicPlayer.loadTrack("level3_track", "resources/music/pista07.ogg");
    musicPlayer.loadTrack("level4_track", "resources/music/pista04.ogg");
    musicPlayer.loadTrack("level5_track", "resources/music/pista09.ogg");
    // Si se añade más pistas seguir el modo de nombrar a cada track (levelx_track) si se trata de un nivel

    musicPlayer.playTrack("menu_track");

    musicPlayer.setVolume(25.f); 

    sf::Clock gameClock;
    const float UPDATE_RATE = 1.0f / 60.0f; // 60 updates por segundo
    float accumulator = 0.0f;
    float lastTime = gameClock.getElapsedTime().asSeconds();

    while (window.isOpen()) {
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float frameTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Evitar spiral of death
        if (frameTime > 0.25f)
        frameTime = 0.25f;
        
        accumulator += frameTime;
        
        // Manejo de input
        stateManager.handleInput(window);
        
        // Actualización con tiempo fijo
        while (accumulator >= UPDATE_RATE) {
            // Actualizar estados del juego
            stateManager.fixedUpdate(window);
            accumulator -= UPDATE_RATE;
        }

        MusicPlayer::getInstance().update(); 

        // Renderizado con interpolación
        window.clear();
        stateManager.renderWithInterpolation(window);

        window.display();
    }

    return 0;
}